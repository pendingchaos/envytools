#include "dis-intern.h"
#include "sched-intern.h"

#include <endian.h>

/* instruction parsing */
static int lookupfile(struct schedtarget *targ, const char *name) {
	unsigned i;
	for (i = 0; i < targ->regfilesnum; i++) {
		if (!strcmp(targ->regfiles[i], name))
			return i;
	}
	return -1;
}

static const struct schedinfo *lookupschedinfo(struct schedtarget *targ, uint64_t opcode) {
	unsigned i;
	for (i = 0; i < targ->schedinfonum; i++) {
		if (targ->schedinfo[i].opcode == opcode)
			return &targ->schedinfo[i];
	}
	return NULL;
}

static int isregwrite(const struct schedinfo *info, unsigned pos) {
	unsigned i;
	for (i = 0; i < ARRAY_SIZE(info->regwr) && info->regwr[i] >= 0; i++) {
		if (info->regwr[i] == pos)
			return 1;
	}
	return 0;
}

static int handletab(struct schedtarget *targ, struct insninfo *info, uint64_t insn, const struct insn *tab, int root);

static int handleatom(struct schedtarget *targ, struct insninfo *info, const struct atom *atom, uint64_t insn) {
	if (atom->fun_dis == atomreg_d) {
		const struct reg *reg = atom->arg;
		assert(!reg->bf->sbf[1].len);
		struct sbf sbf = reg->bf->sbf[0];
		unsigned val = extr(insn, sbf.pos, sbf.len);
		int file = lookupfile(targ, reg->name);
		if (file < 0)
			return 0;
		unsigned size = targ->getregsize(info, sbf.pos);
		unsigned i;
		for (i = val; i < val + size; i++) {
			if (i == targ->regbitbuckets[file])
				continue;
			struct reginfo rinfo;
			rinfo.reg = i;
			rinfo.pos = sbf.pos;
			rinfo.file = file;
			rinfo.wr = isregwrite(info->sched, rinfo.pos);
			rinfo.bar = -1;
			info->regs[info->regcount++] = rinfo;
		}
	} else if (atom->fun_dis == atomtab_d) {
		if (handletab(targ, info, insn, atom->arg, 0))
			return 1;
	} else if (atom->fun_dis == atomunk_d) {
		fprintf(stderr, "Failed to parse binary for scheduling");
		return 1;
	}

	if (targ->atomcallback)
		targ->atomcallback(info, atom);

	return 0;
}

static int handletab(struct schedtarget *targ, struct insninfo *info, uint64_t insn, const struct insn *tab, int root) {
	while (1) {
		if ((insn & tab->mask) != tab->val) {
			tab++;
			continue;
		}
		if (root) {
			info->opcode = tab->val;
			info->sched = lookupschedinfo(targ, info->opcode);
			if (!info->sched) {
				fprintf(stderr, "No scheduling information for instruction: 0x%lxull\n", info->opcode);
				return 1;
			}
		}
		insn &= ~tab->mask;
		unsigned i;
		for (i = 0; i < ARRAY_SIZE(tab->atoms); i++) {
			if (tab->atoms[i].fun_dis)
				handleatom(targ, info, &tab->atoms[i], insn);
		}
		return 0;
	}
}

static int getinsninfo(struct schedtarget *targ, struct insninfo *info, uint64_t insn, const struct insn *tab) {
	memset(info, 0, sizeof(*info));
	info->insn = insn;
	info->rdbar = info->wrbar = -1;
	if (handletab(targ, info, insn, tab, 1))
		return 1;

	if (targ->insncallback)
		targ->insncallback(info);

	return 0;
}

/* scheduling */
struct schedctx {
	struct schedtarget *targ;
	unsigned insnsnum;
	struct insninfo *insns;
	unsigned barnum;
	struct schedinsn *schedinsns; /* one for each instruction */
};

static void createbarriers(struct schedctx *ctx) {
	int latest[SCHED_MAX_REGS][ctx->targ->regfilesnum];
	int latestrd[SCHED_MAX_REGS][ctx->targ->regfilesnum];
	memset(latest, 0xff, sizeof(latest));
	memset(latestrd, 0xff, sizeof(latestrd));
	ctx->barnum = 0;
	unsigned i;
	for (i = 0; ctx->insnsnum && i < ctx->insnsnum - 1; i++) {
		struct insninfo *info = &ctx->insns[i];

		/* create write barriers first since they can make read barriers unnecessary */
		unsigned rdpass;
		for (rdpass = 0; rdpass < 2; rdpass++) {
			unsigned j;
			for (j = 0; j < info->regcount; j++) {
				struct reginfo *r = &info->regs[j];
				int cur = latest[r->file][r->reg];
				if (cur != -1 && ctx->insns[cur].sched->latency < 0) {
					struct insninfo *cinfo = &ctx->insns[cur];

					int needbar = 0;
					if (latestrd[r->file][r->reg] && r->wr && rdpass)
						needbar = 1; /* write after read */
					else if (!latestrd[r->file][r->reg] && r->wr && !rdpass)
						needbar = 1; /* write after write */
					else if (!latestrd[r->file][r->reg] && !r->wr && !rdpass)
						needbar = 1; /* read after write */
					else
						continue;

					/* skip if the read/write barrier has already been created and thus waited on */
					if (rdpass && needbar && (cinfo->rdbar != -1 || cinfo->wrbar != -1))
						continue;
					if (!rdpass && needbar && cinfo->wrbar != -1)
						continue;

					/* "create" and use the barrier */
					if (rdpass)
						r->bar = cinfo->rdbar = ctx->barnum++;
					else
						r->bar = cinfo->wrbar = ctx->barnum++;

					if (cur + 1 == i)
						info->needbarprev = 1;
				}
			}
		}

		unsigned j;
		for (j = 0; j < info->regcount; j++) {
			struct reginfo *r = &info->regs[j];
			if (r->wr)
				continue;
			latest[r->file][r->reg] = i;
			latestrd[r->file][r->reg] = 1;
		}

		/* do writes after reads */
		for (j = 0; j < info->regcount; j++) {
			struct reginfo *r = &info->regs[j];
			if (!r->wr)
				continue;
			latest[r->file][r->reg] = i;
			latestrd[r->file][r->reg] = 0;
		}

		if (info->sched->controlflow) {
			/* Control flow instructions wait on all barriers */
			memset(latest, 0xff, sizeof(latest));
			memset(latestrd, 0xff, sizeof(latestrd));
		}
	}
}

static void createschedinsns(struct schedctx *ctx) {
	ctx->schedinsns = calloc(ctx->insnsnum, sizeof(struct schedinsn));
	unsigned i;
	for (i = 0; i < ctx->insnsnum; i++)
		ctx->schedinsns[i].rdbar = ctx->schedinsns[i].wrbar = 7;

	int ready[SCHED_MAX_REGS][ctx->targ->regfilesnum];
	memset(ready, 0, sizeof(ready));

	int barriers[6];
	memset(barriers, 0, sizeof(barriers));

	unsigned *barmap = calloc(ctx->barnum, sizeof(unsigned));

	int curcycle = 0;

	for (i = 0; i < ctx->insnsnum; i++) {
		struct insninfo *info = &ctx->insns[i];
		const struct schedinfo *sinfo = info->sched;
		struct insninfo *next = i != ctx->insnsnum - 1 ? &ctx->insns[i + 1] : NULL;

		/* wait on barriers */
		unsigned j;
		for (j = 0; ctx->targ->hasbarriers && j < info->regcount; j++) {
			struct reginfo *r = &info->regs[j];
			if (r->bar == -1)
				continue;
			ctx->schedinsns[i].wait |= 1 << barmap[r->bar];
			barriers[barmap[r->bar]] = 0; /* free the barrier for later use */
		}

		/* create barriers */
		if (info->rdbar != -1) {
			assert(ctx->targ->hasbarriers);
			unsigned bar = 0;
			for (; bar < 6; bar++) {
				if (!barriers[bar])
					break;
			}
			barriers[bar] = 1;
			ctx->schedinsns[i].rdbar = bar;

			barmap[info->rdbar] = bar;
		}
		if (info->wrbar != -1) {
			assert(ctx->targ->hasbarriers);
			unsigned bar = 0;
			for (; bar < 6; bar++) {
				if (!barriers[bar])
					break;
			}
			barriers[bar] = 1;
			ctx->schedinsns[i].wrbar = bar;

			barmap[info->wrbar] = bar;
		}

		/* update ready information */
		for (j = 0; j < info->regcount; j++) {
			if (!info->regs[j].wr)
				continue;
			int latency = abs(sinfo->latency);
			latency = max(latency, ctx->targ->regminlatency[info->regs[j].file]);

			if (next && next->needbarprev && ctx->targ->hasbarriers)
				latency = max(latency, 2); /* barriers require two cycles to become active on Maxwell+ */

			ready[info->regs[j].file][info->regs[j].reg] = curcycle + latency;
		}

		/* calculate a delay */
		int delay = sinfo->mindelay;
		for (j = 0; next && j < next->regcount; j++)
			delay = max(delay, ready[next->regs[j].file][next->regs[j].reg] - curcycle);

		if (sinfo->controlflow || i == 0) {
			/* wait on all barriers */
			ctx->schedinsns[i].wait = 0x3f;
			memset(barriers, 0, sizeof(barriers));

			/* wait for all instructions to finish */
			unsigned f;
			for (f = 0; f < ctx->targ->regfilesnum; f++) {
				unsigned r;
				for (r = 0; r < SCHED_MAX_REGS; r++)
					delay = max(delay, ready[f][r] - curcycle);
			}
		}

		delay = min(max(delay, 1), 15);
		ctx->schedinsns[i].delay = delay;

		/* TODO: reuse flag */
		/* TODO: dual issue */

		curcycle += delay;
	}

	free(barmap);
}

int do_sched(const struct disisa *isa, unsigned funcsize, uint64_t *func) {
	struct schedctx ctx;
	ctx.targ = isa->schedtarg;

	int freq = ctx.targ->schedinsnfreq;

	/* parse instructions */
	ctx.insns = calloc(funcsize, sizeof(struct insninfo));
	ctx.insnsnum = funcsize - (funcsize + freq) / (freq + 1);
	unsigned i;
	for (i = 0; i < funcsize; i++) {
		if (i % (freq + 1) == 0) {
			if ((func[i] & ctx.targ->schedinsnmask) != ctx.targ->schedinsnval) {
				fprintf(stderr, "Invalid schedule instruction\n");
				return 1;
			}
			continue;
		}

		unsigned dest = i - (i + 3) / 4;
		if (getinsninfo(ctx.targ, &ctx.insns[dest], le64toh(func[i]), isa->troot)) {
			free(ctx.insns);
			return 1;
		}
	}

	ctx.barnum = 0;
	if (ctx.targ->hasbarriers)
		createbarriers(&ctx);

	createschedinsns(&ctx);

	/* output */
	for (i = 0; i < ctx.insnsnum; i += freq) {
		int destindex = i / freq * (freq + 1);
		int num = min(ctx.insnsnum - i, freq);
		func[destindex] = ctx.targ->encschedinsn(num, ctx.schedinsns + i);
	}

	/* cleanup */
	free(ctx.schedinsns);
	free(ctx.insns);

	return 0;
}
