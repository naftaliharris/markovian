// hashgen.c
// generates random numbers to use in hashing

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SEED 67783234

uint64_t random64(void)
{
	uint64_t result = random();
	result = result << 32;
	result |= random();
	return (result);
}

int main(void)
{
	char *states = malloc(1024 * 1024);	//frivolous, but whatever
	initstate(SEED, states, 1024 * 1024);

	fprintf(stdout,
		"// hashrands.h\n// random numbers to use in hashing\n\n");

	fprintf(stdout, "#define WKingHash	Hash[0]\n");
	fprintf(stdout, "#define BKingHash	Hash[1]\n");
	fprintf(stdout, "#define WQueensHash	Hash[2]\n");
	fprintf(stdout, "#define BQueensHash	Hash[3]\n");
	fprintf(stdout, "#define WRooksHash	Hash[4]\n");
	fprintf(stdout, "#define BRooksHash	Hash[5]\n");
	fprintf(stdout, "#define WBishopsHash	Hash[6]\n");
	fprintf(stdout, "#define BBishopsHash	Hash[7]\n");
	fprintf(stdout, "#define WKnightsHash	Hash[8]\n");
	fprintf(stdout, "#define BKnightsHash	Hash[9]\n");
	fprintf(stdout, "#define WPawnsHash	Hash[10]\n");
	fprintf(stdout, "#define BPawnsHash	Hash[11]\n");
	//fprintf(stdout,"#define EPHash                Hash[12]\n");

	fprintf(stdout, "\n");

	fprintf(stdout, "uint64_t ToMoveHash[2] = {\n");
	for (int i = 0; i < 2; i++) {
		fprintf(stdout, "0x%llx,\n", random64());
	}
	fprintf(stdout, "};\n\n");

	fprintf(stdout, "uint64_t EPHash[9] = {\n");
	for (int i = 0; i < 9; i++) {
		fprintf(stdout, "0x%llx,\n", random64());
	}
	fprintf(stdout, "};\n\n");

	fprintf(stdout, "uint64_t CastlingHash[16] = {\n");
	for (int i = 0; i < 16; i++) {
		fprintf(stdout, "0x%llx,\n", random64());
	}
	fprintf(stdout, "};\n\n");

	fprintf(stdout, "uint64_t Hash[12][64] = {\n");
	for (int i = 0; i < 12; i++) {
		fprintf(stdout, "{\n");
		for (int j = 0; j < 64; j++) {
			fprintf(stdout, "0x%llx,\n", random64());
		}
		fprintf(stdout, "},\n");
	}
	fprintf(stdout, "};\n\n");

	return (0);
}
