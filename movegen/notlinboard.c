// notlinboard.c
// generates ~linboard[]

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../bitboard.h"

int main(void)
{
	fprintf(stdout,"const uint64_t notlinboard[64] = {\n");
	for(int i = 0; i < 64; i++){
		fprintf(stdout,"0x%llx,\n",~linboard[i]);
	}
	fprintf(stdout,"};\n");

	return(0);
}
