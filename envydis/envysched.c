#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "envyas.h"
#include "util.h"

static void printhelp(char **argv) {
	fprintf(stderr, "Usage: %s [args ...]\n", argv[0]);
	fprintf(stderr, "\n");
	fprintf(stderr, "Reschedules machine code from stdin, writing the result to stdout.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "-m <machine> Select the ISA of the input code\n");
}

int main(int argc, char **argv) {
	const struct disisa *isa = NULL;;
	int c;
	while ((c = getopt(argc, argv, "m:h")) != -1) {
		switch (c) {
			case 'm':
				isa = ed_getisa(optarg);
				if (!isa || !isa->schedtarg) {
					fprintf(stderr, "Unknown or unsupported architecture\n");
					return 1;
				}
				break;
			case 'h':
				printhelp(argv);
				return 1;
			case ':':
			case '?':
				fprintf(stderr, "Incorrect arguments\n");
				return 1;
		}
	}
	if (!isa) {
		fprintf(stderr, "No architecture specified\n");
		return 1;
	}

	/* pretty inefficient but quick enough */
	int insnsnum = 0, insnsmax = 0;
	uint64_t *insns = NULL;
	uint64_t insn;
	while (fread(&insn, 8, 1, stdin) == 1)
		ADDARRAY(insns, insn);
	if (ferror(stdin)) {
		fprintf(stderr, "Failed to read instructions from stdin: %s\n", strerror(errno));
		return 1;
	}

	if (do_sched(isa, insnsnum, insns)) {
		fprintf(stderr, "Failed to schedule instructions.\n");
		fprintf(stderr, "If it isn't because of an unsupported instruction, try running the code through\n");
		fprintf(stderr, "envydis and seeing if anything looks wrong.\n");
		return 1;
	}

	if (fwrite(insns, insnsnum, 8, stdout) != insnsnum) {
		fprintf(stderr, "Failed to write instructions to stdout: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
