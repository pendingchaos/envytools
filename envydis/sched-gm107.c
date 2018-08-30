#include "sched-intern.h"

static const char * const gm107_regfiles[] = {"r", "p", ""/*< flags register*/};
static const int gm107_regbitbuckets[] = {255, 7, -1};
static const int gm107_regminlatency[] = {0, 13, 0};

static const struct schedinfo gm107_schedinfo[] = {
	{"xmad", 0x5b00000000000000ull, {0, -1}, 6},
	{"xmad", 0x5100000000000000ull, {0, -1}, 6},
	{"xmad", 0x4e00000000000000ull, {0, -1}, 6},
	{"iadd", 0x5c10000000000000ull, {0, -1}, 6},
	{"iadd", 0x4c10000000000000ull, {0, -1}, 6},
	{"iadd", 0x3810000000000000ull, {0, -1}, 6},
	{"fadd", 0x5c58000000000000ull, {0, -1}, 6},
	{"fadd", 0x4c58000000000000ull, {0, -1}, 6},
	{"fadd", 0x3858000000000000ull, {0, -1}, 6},
	{"fmul", 0x5c68000000000000ull, {0, -1}, 6},
	{"fmul", 0x4c68000000000000ull, {0, -1}, 6},
	{"fmul", 0x3868000000000000ull, {0, -1}, 6},
	{"ffma", 0x5980000000000000ull, {0, -1}, 6},
	{"ffma", 0x5180000000000000ull, {0, -1}, 6},
	{"ffma", 0x4980000000000000ull, {0, -1}, 6},
	{"ffma", 0x3280000000000000ull, {0, -1}, 6},
	{"imul", 0x5c38000000000000ull, {0, -1}, -6},
	{"imul", 0x4c38000000000000ull, {0, -1}, -6},
	{"imul", 0x3838000000000000ull, {0, -1}, -6},
	{"imad", 0x5a00000000000000ull, {0, -1}, -6},
	{"imad", 0x5200000000000000ull, {0, -1}, -6},
	{"imad", 0x4a00000000000000ull, {0, -1}, -6},
	{"imad", 0x3400000000000000ull, {0, -1}, -6},
	{"flo", 0x5c30000000000000ull, {0, -1}, -13},
	{"flo", 0x4c30000000000000ull, {0, -1}, -13},
	{"flo", 0x3830000000000000ull, {0, -1}, -13},
	{"lop", 0x5c40000000000000ull, {0, 48, -1}, 6},
	{"lop", 0x4c40000000000000ull, {0, 48, -1}, 6},
	{"lop", 0x3840000000000000ull, {0, 48, -1}, 6},
	{"mov", 0x5c98000000000000ull, {0, -1}, 6},
	{"mov", 0x4c98000000000000ull, {0, -1}, 6},
	{"mov", 0x3898000000000000ull, {0, -1}, 6},
	{"shl", 0x5c48000000000000ull, {0, -1}, 6},
	{"shl", 0x4c48000000000000ull, {0, -1}, 6},
	{"shl", 0x3848000000000000ull, {0, -1}, 6},
	{"i2i", 0x5ce0000000000000ull, {0, -1}, -6},
	{"i2i", 0x4ce0000000000000ull, {0, -1}, -6},
	{"i2i", 0x38e0000000000000ull, {0, -1}, -6},
	{"isetp", 0x5b60000000000000ull, {0, 3, -1}, 6},
	{"isetp", 0x4b60000000000000ull, {0, 3, -1}, 6},
	{"isetp", 0x3660000000000000ull, {0, 3, -1}, 6},
	{"ipa", 0xe00000000000ff00ull, {0, -1}, -15},
	{"ipa", 0xe000004000000000ull, {0, -1}, -15},
	{"mufu", 0x5080000000000000ull, {0, -1}, -13},
	{"tex", 0xdeb8000000000000ull, {0, -1}, -15},
	{"tex", 0xc038000000000000ull, {0, -1}, -15},
	{"depbar", 0xf0f0000000000000ull, {-1}, 0},
	{"ld", 0xefd8000000000000ull, {0, -1}, -15},
	{"st", 0xeff0000000000000ull, {-1}, -15},
	{"ld", 0xef90000000000000ull, {0, -1}, -15},
	{"nop", 0x50b0000000000000ull, {-1}, 0},
	{"bra", 0xe240000000000000ull, {-1}, 0, 1, 13},
	{"ret", 0xe320000000000000ull, {-1}, 0, 1, 13},
	{"exit", 0xe300000000000000ull, {-1}, 0, 1, 15},
};

static unsigned gm107_getregsize(const struct insninfo *info, unsigned pos) {
	if (info->opcode == 0xdeb8000000000000ull || info->opcode == 0xc038000000000000ull) {
		/* tex */
		switch (pos) {
		case 0: {
			unsigned mask = (info->insn >> 31) & 0xf;
			/* use a LUT */
			return (0x8da691691448llu >> (mask * 3)) & 7;
		}
		case 8: {
			int shadow = (info->insn >> 50) & 1;
			int type = (info->insn >> 29) & 3; //0 = 1d, 1 = 2d, 2 = 3d, 3 = cube
			int array = (info->insn >> 28) & 1;
			assert(!shadow && "Shadow texture samples not handled");
			return array + min(type + 1, 3);
		}
		}
	} else if (info->opcode == 0xefd8000000000000ull || info->opcode == 0xeff0000000000000ull) {
		/* attribute load/store instruction */
		if (pos == 0)
			return ((info->insn >> 47) & 3) + 1;
	} else if (info->opcode == 0xef90000000000000ull) {
		/* constant buffer load instruction */
		if (pos == 0)
			return ((info->insn >> 48) & 7) == 5 ? 2 : 1;
	}

	return 1;
}

static void gm107_atomcallback(struct insninfo *info, const struct atom *atom) {
	if (atom->fun_dis == atomname_d) {
		if (!strcmp(atom->arg, "cc")) {
			struct reginfo rinfo = {0, 2, -1, 1, -1};
			info->regs[info->regcount++] = rinfo;
		} else if (!strcmp(atom->arg, "x")) {
			struct reginfo rinfo = {0, 2, -1, 0, -1};
			info->regs[info->regcount++] = rinfo;
		}
	}
}

static uint64_t gm107_encschedinsn(int num, struct schedinsn *insns) {
	uint64_t res = 0;
	int i;
	for (i = 0; i < num; i++) {
		int shift = i * 21;
		res |= (uint64_t)insns[i].delay << shift;
		res |= (uint64_t)insns[i].wrbar << (shift + 5);
		res |= (uint64_t)insns[i].rdbar << (shift + 8);
		res |= (uint64_t)insns[i].wait << (shift + 11);
	}
	for (; i < 3; i++)
	    res |= 0x7efllu << (i * 21);
	return res;
}

static int gm107_inssched(int *pos, uint8_t *code) {
	if (!pos && !code)
		return 8;

	if (*pos % 32 == 0) {
		if (code)
			memset(code + *pos, 0, 8);
		*pos += 8;
	}

	return 0; /* meaningless and shouldn't be used */
}

static int gm107_inspaddingnop(int *pos, uint8_t *code) {
	if (!pos && !code)
		return 8;

	if (*pos % 32 == 0)
		return 0;

	if (code) {
		static const char *nop = "\x00\x0f\x07\x00\x00\x00\xb0\x50";
		memcpy(code + *pos, nop, 8);
	}
	*pos += 8;

	return 1;
}

struct schedtarget gm107_target = {
	.schedinfonum = ARRAY_SIZE(gm107_schedinfo),
	.schedinfo = gm107_schedinfo,

	.schedinsnmask = 0x0,
	.schedinsnval = 0x0,
	.schedinsnfreq = 3,

	.regfilesnum = ARRAY_SIZE(gm107_regfiles),
	.regfiles = gm107_regfiles,
	.regbitbuckets = gm107_regbitbuckets,
	.regminlatency = gm107_regminlatency,

    .hasbarriers = 1,

	.getregsize = &gm107_getregsize,
	.atomcallback = &gm107_atomcallback,
	.insncallback = NULL,
	.encschedinsn = &gm107_encschedinsn,

	.inssched = &gm107_inssched,
	.inspaddingnop = &gm107_inspaddingnop
};
