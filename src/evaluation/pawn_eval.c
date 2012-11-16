// pawn_eval.c
// generates pawn structure evaluation
// scores undefended and passed pawns

#ifndef _PAWN_EVAL_C
#define _PAWN_EVAL_C

#include "doubled_penalty.h"
#include "passed_points.h"
#include "squares.h"
//#define H8 9223372036854775808

uint32_t pawn_eval(struct position *);
inline uint32_t pawn_eval(struct position *pos)
{
	uint32_t result = 0;

	uint64_t wpawn_file[8], bpawn_file[8];

	// generate pawn files
	for (int j = 0; j < 8; j++) {
		wpawn_file[j] = pos->wpawns & file[j];
		bpawn_file[j] = pos->bpawns & file[j];
	}

	// doubled pawns
	// consider changing the if statements to multiplicative things: result += doubled_penalty[j] * max(sparse_popcount(.) - 1, 0);
	//
	// or perhaps:
	// x = sparse_popcount(.);
	// result += (x * (x-1) >> 2) * penalty.
	//
	// x | x * (x-1) >> 1
	// 0 | 0
	// 1 | 0
	// 2 | 1
	// 3 | 3

	// lookups might be faster...

	int x;
#ifdef _ESTIMATE_PAWN_EVAL
	int y;
#endif
	for (int j = 0; j < 8; j++) {
		x = sparse_popcount(wpawn_file[j]);
		result += ((x * (x - 1)) >> 1) * doubled_penalty[j];
#ifdef _ESTIMATE_PAWN_EVAL
		y = (x * (x - 1)) >> 1;
#endif

		x = sparse_popcount(bpawn_file[j]);
		result -= ((x * (x - 1)) >> 1) * doubled_penalty[j];
#ifdef _ESTIMATE_PAWN_EVAL
		y -= (x * (x - 1)) >> 1;
		fprintf(stdout, "%d ", y);
#endif

		/*
		   if(sparse_popcount(wpawn_file[j]) >= 2){
		   result += doubled_penalty[j];
		   }
		   if(sparse_popcount(bpawn_file[j]) >= 2){
		   result -= doubled_penalty[j];
		   }
		 */
	}

	/*
	   // UNDEFENDED PAWN DETECTION: (?)
	   uint64_t undefended;

	   // white undefended pawns
	   undefended = pos->wpawns ^ ((moveNW(pos->wpawns) & ~file[7]) | (moveNE(pos->wpawns) & ~file[0]));

	   // black undefended pawns
	   undefended = pos->bpawns ^ ((moveSW(pos->bpawns) & ~file[7]) | (moveSE(pos->bpawns) & ~file[0]));
	 */

	// PASSED PAWN DETECTION
	// can we save on ffslling somehow?
	uint64_t obstructions;	//all enemy and friendly pawns, all enemy pawn attack squares
	uint64_t locs;
	int index_from;

#ifdef _ESTIMATE_PAWN_EVAL
	char *squares = calloc(64, 1);
#endif

	// white passed pawns
	obstructions =
	    pos->wpawns | pos->
	    bpawns | ((moveSW(pos->bpawns) & ~file[7]) |
		      (moveSE(pos->bpawns) & ~file[0]));
	locs = pos->wpawns;
	for (int i = 0; i < 8; i++) {
		if ((index_from = ffsll(locs)) != 0) {
			index_from--;
			if ((vectorN[index_from] & obstructions)
			    || (pawn_attacks[BLACK][index_from] & pos->
				bpawns)) {
				continue;
			} else {
				result += WPassed_points[rank(index_from)];
#ifdef _ESTIMATE_PAWN_EVAL
				squares[index_from] += 1;
#endif
			}
			locs &= notlinboard[index_from];
		} else {
			break;
		}
	}

	// black passed pawns
	obstructions =
	    pos->bpawns | pos->
	    wpawns | ((moveNW(pos->wpawns) & ~file[7]) |
		      (moveNE(pos->wpawns) & ~file[0]));
	locs = pos->bpawns;
	for (int i = 0; i < 8; i++) {
		if ((index_from = ffsll(locs)) != 0) {
			index_from--;
			if ((vectorS[index_from] & obstructions)
			    || (pawn_attacks[WHITE][index_from] & pos->
				wpawns)) {
				continue;
			} else {
				result -= BPassed_points[rank(index_from)];
#ifdef _ESTIMATE_PAWN_EVAL
				squares[index_from] -= 1;
#endif
			}
			locs &= notlinboard[index_from];
		} else {
			break;
		}
	}

#ifdef _ESTIMATE_PAWN_EVAL
	print_squares_comp(squares);
	free(squares);
#endif

	return (result);
}

#endif
