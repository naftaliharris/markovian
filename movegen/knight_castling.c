// knight_castling.c
// Genenerates knight positions that are impediments to castling

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "knightmove.h"

int main(int argc, char **argv){

	fprintf(stdout,"// knight_castling.h\n// constants for where knights may attack castling squares\n// In the following, Knight_Castling[0] = WKING, Knight_Castling[1] = WQUEEN, Knight_Castling[2] = BKING, ...[3] = BQUEEN\n\n#define KC_WKING 0\n#define KC_WQUEEN 1\n#define KC_BKING 2\n#define KC_BQUEEN 3\n\n");
	uint64_t Knight_Castling[4] = {
	(knightmoves[4 + 8*0] | knightmoves[5 + 8*0] | knightmoves[6 + 8*0]),
	(knightmoves[2 + 8*0] | knightmoves[3 + 8*0] | knightmoves[4 + 8*0]),
	(knightmoves[4 + 8*7] | knightmoves[5 + 8*7] | knightmoves[6 + 8*7]),
	(knightmoves[2 + 8*7] | knightmoves[3 + 8*7] | knightmoves[4 + 8*7]),
	};


	fprintf(stdout, "const uint64_t Knight_Castling[4] = {\n");
	for(int i = 0; i < 4; i++){
		fprintf(stdout, "0x%llx,\n",(long long unsigned) Knight_Castling[i]);
	}
	fprintf(stdout,"};\n");
        return 0;
}
