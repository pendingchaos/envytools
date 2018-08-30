#ifndef SCHED_INTERN_H
#define SCHED_INTERN_H

#include "dis-intern.h"

#define SCHED_MAX_REGS 256

struct reginfo {
	unsigned reg;
	unsigned file;
	unsigned pos; //position within instruction
	int wr;

	// members initialized during and for scheduling
	int bar; //virtual barrier to wait on or -1
};

struct schedinfo {
	const char *name;
	uint64_t opcode;
	int regwr[16];
	int latency; /* negative if it requires barriers */
	int controlflow;
	int mindelay;
};

struct insninfo {
	uint64_t insn;
	const struct schedinfo *sched;
	uint64_t opcode;
	unsigned regcount;
	struct reginfo regs[64];

	// members initialized during and for scheduling
	int needbarprev; //whether it needs a barrier created by the previous instruction
	int rdbar; //virtual read barrier created or -1
	int wrbar; //virtual write barrier created or -1
};

/* scheduling data for one instruction */
struct schedinsn {
	unsigned delay:4;
	unsigned rdbar:3; /* 7 if none is created */
	unsigned wrbar:3; /* 7 if none is created */
	unsigned wait:6;
};

struct schedtarget {
	unsigned schedinfonum;
	const struct schedinfo *schedinfo;

	uint64_t schedinsnmask;
	uint64_t schedinsnval;
	unsigned schedinsnfreq;

	unsigned regfilesnum;
	const char *const* regfiles;
	const int *regbitbuckets;
	const int *regminlatency;

	int hasbarriers;

	unsigned (*getregsize)(const struct insninfo *info, unsigned pos);
	void (*atomcallback)(struct insninfo *info, const struct atom *atom);
	void (*insncallback)(struct insninfo *info);
	uint64_t (*encschedinsn)(int num, struct schedinsn *insns);

	int (*inssched)(int *pos, uint8_t *code);
	int (*inspaddingnop)(int *pos, uint8_t *code);
};

#endif
