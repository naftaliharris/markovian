// position.c
// elementary functions to compute on positions

#ifndef _POSITION_C
#define _POSITION_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "position.h"
#include "hash/hash.c"

char data_to_human(struct position *pos, unsigned char i, unsigned char j)
{
	uint64_t piece = board[i][j];
	if (piece & pos->wpawns) {
		return 'p';
	} else if (piece & pos->wbishops) {
		return 'b';
	} else if (piece & pos->wknights) {
		return 'n';
	} else if (piece & pos->wrooks) {
		return 'r';
	} else if (piece & pos->wqueens) {
		return 'q';
	} else if (piece & pos->wking) {
		return 'k';
	} else if (piece & pos->bpawns) {
		return 'P';
	} else if (piece & pos->bbishops) {
		return 'B';
	} else if (piece & pos->bknights) {
		return 'N';
	} else if (piece & pos->brooks) {
		return 'R';
	} else if (piece & pos->bqueens) {
		return 'Q';
	} else if (piece & pos->bking) {
		return 'K';
	} else {
		return ' ';
	}
}

void print_position(struct position *pos)
{
	fprintf(stdout, "---------------------------------\n");
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			fprintf(stdout, "| %c ", data_to_human(pos, i, j));
		}
		fprintf(stdout, "|\n");
		fprintf(stdout, "---------------------------------\n");
	}
}

void print_bitboard(uint64_t bitboard)
{
	fprintf(stdout, "---------------------------------\n");
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (bitboard & board[i][j]) {
				fprintf(stdout, "| 1 ");
			} else {
				fprintf(stdout, "|   ");
			}
		}
		fprintf(stdout, "|\n");
		fprintf(stdout, "---------------------------------\n");
	}
}

void init_position(struct position *pos)
{
	memset(pos, 0, sizeof(struct position));
	for (int i = 0; i < 8; i++) {
		pos->wpawns |= board[1][i];
		pos->bpawns |= board[6][i];
	}
	pos->wrooks |= board[0][0];
	pos->wrooks |= board[0][7];
	pos->brooks |= board[7][0];
	pos->brooks |= board[7][7];
	pos->wknights |= board[0][1];
	pos->wknights |= board[0][6];
	pos->bknights |= board[7][1];
	pos->bknights |= board[7][6];
	pos->wbishops |= board[0][2];
	pos->wbishops |= board[0][5];
	pos->bbishops |= board[7][2];
	pos->bbishops |= board[7][5];
	pos->wqueens |= board[0][3];
	pos->wking |= board[0][4];
	pos->bqueens |= board[7][3];
	pos->bking |= board[7][4];

	pos->wpieces =
	    pos->wrooks | pos->wknights | pos->wbishops | pos->wqueens | pos->
	    wking | pos->wpawns;
	pos->bpieces =
	    pos->brooks | pos->bknights | pos->bbishops | pos->bqueens | pos->
	    bking | pos->bpawns;
	pos->allpieces = pos->wpieces | pos->bpieces;

	pos->tomove = WHITE;
	pos->towait = BLACK;
	pos->perm_castling = ALL;
	pos->ep = EP_NONE;
	pos->score = 0;
	pos->tot_mat =
	    2 * QUEEN_VAL + 4 * ROOK_VAL + 4 * BISHOP_VAL + 4 * KNIGHT_VAL +
	    16 * PAWN_VAL;
	pos->move_num = 0;
	pos->limp1 = 0;
	pos->half_move_clock = 0;

	pos->hash = make_hash(pos);
}

#ifdef _DEBUG
// makes sure board is consistent
int consistency(struct position *pos)
{
	if (pos->wpieces != (pos->wrooks | pos->wknights | pos->wbishops | pos->wqueens | pos->wking | pos->wpawns)) {
		fprintf(stderr, "wpieces inconsistent.\n");
		return (0);
	}
	if (pos->bpieces != (pos->brooks | pos->bknights | pos->bbishops | pos->bqueens | pos-> bking | pos->bpawns)) {
		fprintf(stderr, "bpieces inconsistent.\n");
		return (0);
	}
	if (! ((pos->tomove == WHITE && pos->towait == BLACK)
	     || (pos->tomove == BLACK && pos->towait == WHITE))) {
		fprintf(stderr, "tomove and towait inconsistent.\n");
		return (0);
	}
	if (pos->allpieces != (pos->wpieces | pos->bpieces)) {
		fprintf(stderr, "allpieces inconsistent.\n");
		return (0);
	}
	if ((ep_squares[pos->towait][pos->ep] & pos->allpieces)) {
		fprintf(stderr, "piece on ep square: inconsistent.\n");
		return (0);
	}
	if (pos->hash != make_hash(pos)) {
		fprintf(stderr, "hash inconsistent.\n");
		return (0);
	}

	return (1);
}
#endif

#endif
