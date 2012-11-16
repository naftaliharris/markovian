// control_eval.c
// Returns who's winning in square control
// Each function only considers possible attacks, ie, char *square holds the net number of times the squares are attacked, (including own-piece attacks), and pawn non-attacks don't count. Pawns may attack empty squares

#ifndef _CONTROL_EVAL_C
#define _CONTROL_EVAL_C

#include "control_params.h"
#include "squares.h"

void rook_control(struct position *, unsigned char, char *);
inline void rook_control(struct position *pos, unsigned char rooks,
			 char *squares)
{
	int multiplier = -2 * (rooks & COLOR) + 1;	//white = 1, black = -1
	uint64_t rook_pos = pos->pieces[rooks];
	uint64_t moves;
	unsigned char index_from, index_to;
	for (int i = 0; i < 10; i++) {	// up to 10 rooks of same color on the board, (for loop for compiler loop unrolling)
		if ((index_from = ffsll(rook_pos)) != 0) {
			index_from--;
			moves =
			    RookMoves[index_from][magictransform
						  ((pos->
						    allpieces &
						    RookMasks[index_from]),
						   RookMagic[index_from],
						   RookMagicSize[index_from])];
			for (int i = 0; i < 14; i++) {
				if ((index_to = ffsll(moves)) != 0) {
					index_to--;
					squares[index_to] += multiplier * 1;
					moves &= notlinboard[index_to];
				} else {
					break;
				}
			}
			rook_pos &= notlinboard[index_from];
		} else {
			break;
		}
	}
}

void bishop_control(struct position *, unsigned char, char *);
inline void bishop_control(struct position *pos, unsigned char bishops,
			   char *squares)
{
	int multiplier = -2 * (bishops & COLOR) + 1;	//white = 1, black = 0
	uint64_t bishop_pos = pos->pieces[bishops];
	uint64_t moves;
	unsigned char index_from, index_to;
	for (int i = 0; i < 10; i++) {	// up to 10 bishops of same color on the board, (for loop for compiler loop unrolling)
		if ((index_from = ffsll(bishop_pos)) != 0) {
			index_from--;
			moves =
			    BishopMoves[index_from][magictransform
						    ((pos->
						      allpieces &
						      BishopMasks[index_from]),
						     BishopMagic[index_from],
						     BishopMagicSize
						     [index_from])];
			for (int i = 0; i < 14; i++) {
				if ((index_to = ffsll(moves)) != 0) {
					index_to--;
					squares[index_to] += multiplier * 1;
					moves &= notlinboard[index_to];
				} else {
					break;
				}
			}
			bishop_pos &= notlinboard[index_from];
		} else {
			break;
		}
	}

}

void knight_control(struct position *, unsigned char, char *);
inline void knight_control(struct position *pos, unsigned char knights,
			   char *squares)
{
	int multiplier = -2 * (knights & COLOR) + 1;	//white = 1, black = 0
	uint64_t knight_pos = pos->pieces[knights];
	uint64_t moves;
	unsigned char index_from, index_to;
	for (int i = 0; i < 10; i++) {	// can have up to 10 knights of same color on the board, (for loop for compiler loop unrolling)
		if ((index_from = ffsll(knight_pos)) != 0) {
			index_from--;
			moves = knightmoves[index_from];
			for (int i = 0; i < 8; i++) {
				if ((index_to = ffsll(moves)) != 0) {
					index_to--;
					squares[index_to] += multiplier * 1;
					moves &= notlinboard[index_to];
				} else {
					break;
				}
			}
			knight_pos &= notlinboard[index_from];
		} else {
			break;
		}
	}
}

void king_control(struct position *, unsigned char, char *);
inline void king_control(struct position *pos, unsigned char king,
			 char *squares)
{
	int multiplier = -2 * (king & COLOR) + 1;	//white = 1, black = 0
	unsigned char from = ffsll(pos->pieces[king]) - 1;
	uint64_t moves = kingmoves[from];

	unsigned char index;
	for (int i = 0; i < 8; i++) {
		if ((index = ffsll(moves)) != 0) {
			index--;
			squares[index] += multiplier * 1;
			moves &= notlinboard[index];
		} else {
			break;
		}
	}
}

void pawn_control(struct position *, unsigned char, char *);
inline void pawn_control(struct position *pos, unsigned char pawns,
			 char *squares)
{
	int multiplier = -2 * (pawns & COLOR) + 1;	//white = 1, black = 0
	uint64_t pawn_pos = pos->pieces[pawns];
	uint64_t moves;

	unsigned char index_to;
	switch (pawns & COLOR) {
	case WHITE:
		// attack west moves
		pawn_pos = pos->pieces[pawns];
		moves = moveNW(pawn_pos) & ~file[7];
		for (int i = 0; i < 8; i++) {
			if ((index_to = ffsll(moves)) != 0) {
				index_to--;
				squares[index_to] += multiplier * 1;
				moves &= notlinboard[index_to];
			} else {
				break;
			}
		}

		// attack east moves
		pawn_pos = pos->pieces[pawns];
		moves = moveNE(pawn_pos) & ~file[0];
		for (int i = 0; i < 8; i++) {
			if ((index_to = ffsll(moves)) != 0) {
				index_to--;
				squares[index_to] += multiplier * 1;
				moves &= notlinboard[index_to];
			} else {
				break;
			}
		}

		break;
	default:		//black
		// attack west moves
		pawn_pos = pos->pieces[pawns];
		moves = moveSW(pawn_pos) & ~file[7];
		for (int i = 0; i < 8; i++) {
			if ((index_to = ffsll(moves)) != 0) {
				index_to--;
				squares[index_to] += multiplier * 1;
				moves &= notlinboard[index_to];
			} else {
				break;
			}
		}

		// nonpromotion attack east moves
		pawn_pos = pos->pieces[pawns];
		moves = moveSE(pawn_pos) & ~file[0];
		for (int i = 0; i < 8; i++) {
			if ((index_to = ffsll(moves)) != 0) {
				index_to--;
				squares[index_to] += multiplier * 1;
				moves &= notlinboard[index_to];
			} else {
				break;
			}
		}
	}
}

// return board of all net attacks
char *net_attacks(struct position *);
inline char *net_attacks(struct position *pos)
{
	char *squares = calloc(64, 1);	//must be freed by caller

	rook_control(pos, wqueens_n, squares);
	bishop_control(pos, wqueens_n, squares);
	bishop_control(pos, wbishops_n, squares);
	knight_control(pos, wknights_n, squares);
	rook_control(pos, wrooks_n, squares);
	pawn_control(pos, wpawns_n, squares);
	king_control(pos, wking_n, squares);

	rook_control(pos, bqueens_n, squares);
	bishop_control(pos, bqueens_n, squares);
	bishop_control(pos, bbishops_n, squares);
	knight_control(pos, bknights_n, squares);
	rook_control(pos, brooks_n, squares);
	pawn_control(pos, bpawns_n, squares);
	king_control(pos, bking_n, squares);

	return (squares);
}

void print_net_attacks(struct position *pos)
{
	char *squares = net_attacks(pos);
	print_squares(squares);
	free(squares);
}

#define KING_SQUARE_POINTS (PAWN_VAL / 20)

uint32_t king_safety(struct position *, char *);
inline uint32_t king_safety(struct position *pos, char *squares)
{
	uint32_t result = 0;
	unsigned char from;
	unsigned char index;
	uint64_t moves;

	// white king
	from = ffsll(pos->wking) - 1;
	moves = kingmoves[from];

	for (int i = 0; i < 8; i++) {
		if ((index = ffsll(moves)) != 0) {
			index--;
			result += squares[index] * KING_SQUARE_POINTS;
			moves &= notlinboard[index];
		} else {
			break;
		}
	}

	// black king
	from = ffsll(pos->wking) - 1;
	moves = kingmoves[from];

	for (int i = 0; i < 8; i++) {
		if ((index = ffsll(moves)) != 0) {
			index--;
			result += squares[index] * KING_SQUARE_POINTS;
			moves &= notlinboard[index];
		} else {
			break;
		}
	}

	return (result);
}

uint32_t control_eval(struct position *);
inline uint32_t control_eval(struct position *pos)
{
	char *squares = net_attacks(pos);

	// compute t(net_attacks) %*% attack_weights
	uint32_t result = 0;
	for (int i = 0; i < 64; i++) {
		result += squares[i] * control_params[i];
	}

	free(squares);
	return (result);
}

#endif
