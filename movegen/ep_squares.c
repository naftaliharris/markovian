// ep_squares.c
// generates squares on which en passant capture may occur

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../bitboard.h"

int main(void)
{
	fprintf(stdout,"// ep_squares.h\n// squares on which an en passant capture may occur\n// ep_squares[COLOR][file] is the square to which a pawn of notCOLOR may move and kill a pawn of COLOR.\n// file = 8 means no captures are possible, so ep_squares[COLOR][8] = 0\n\n");

	fprintf(stdout,"const uint64_t ep_squares[2][9] = {\n");

	fprintf(stdout,"{\n");
	for(int j = 0; j < 8; j++){
		fprintf(stdout,"0x%llx,\n",board[2][j]);
	}
	fprintf(stdout,"0x0,\n");
	fprintf(stdout,"},\n");

	fprintf(stdout,"{\n");
	for(int j = 0; j < 8; j++){
		fprintf(stdout,"0x%llx,\n",board[5][j]);
	}
	fprintf(stdout,"0x0,\n");
	fprintf(stdout,"},\n");


	fprintf(stdout,"};\n");

	return(0);
}
