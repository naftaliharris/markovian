// makemove.c
// contains the make_move function, which applies a move to a position

#ifndef _MAKEMOVE_C
#define _MAKEMOVE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minmax.h"
#include "position.h"
#include "move.h"

#include "hash/hash.h"
#define CASTLING_BONUS (PAWN_VAL / 4)

void make_move(struct position *, struct move *);
inline void make_move(struct position *pos, struct move *mv)
{
	switch(mv->castling)
	{
		case NONE:
			assert(mv->from < 64);
			assert(mv->to < 64);
			// change material score
			// could be optimized to exploit symmetries
			switch(mv->to_p)
			{
				case nopiece_n:
					break;
				case wpawns_n:
					pos->score -= PAWN_VAL;
					pos->tot_mat -= PAWN_VAL;
					pos->wpawns &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bpawns_n:
					pos->score += PAWN_VAL;
					pos->tot_mat -= PAWN_VAL;
					pos->bpawns &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case wbishops_n:
					pos->score -= BISHOP_VAL;
					pos->tot_mat -= BISHOP_VAL;
					pos->wbishops &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bbishops_n:
					pos->score += BISHOP_VAL;
					pos->tot_mat -= BISHOP_VAL;
					pos->bbishops &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case wknights_n:
					pos->score -= KNIGHT_VAL;
					pos->tot_mat -= KNIGHT_VAL;
					pos->wknights &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bknights_n:
					pos->score += KNIGHT_VAL;
					pos->tot_mat -= KNIGHT_VAL;
					pos->bknights &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case wrooks_n:
					pos->score -= ROOK_VAL;
					pos->tot_mat -= ROOK_VAL;
					pos->wrooks &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;

					//change castling permissions
					switch(mv->to)
					{
						case 0:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= WQUEEN;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						case 7:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= WKING;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
					}
					break;
				case brooks_n:
					pos->score += ROOK_VAL;
					pos->tot_mat -= ROOK_VAL;
					pos->brooks &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;

					//change castling permissions
					switch(mv->to)
					{
						case 56:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= BQUEEN;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						case 63:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= BKING;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
					}
					break;
				case wqueens_n:
					pos->score -= QUEEN_VAL;
					pos->tot_mat -= QUEEN_VAL;
					pos->wqueens &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bqueens_n:
					pos->score += QUEEN_VAL;
					pos->tot_mat -= QUEEN_VAL;
					pos->bqueens &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case wking_n:
					pos->score -= KING_VAL;
					pos->wking &= notlinboard[mv->to];
					pos->wpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bking_n:
					pos->score += KING_VAL;
					pos->bking &= notlinboard[mv->to];
					pos->bpieces &= notlinboard[mv->to];
					pos->hash ^= Hash[mv->to_p][mv->to];
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case eppiece_n:
					switch(pos->tomove)
					{
						case WHITE: //capturing black
							pos->score += PAWN_VAL;
							pos->tot_mat -= PAWN_VAL;
							pos->bpawns &= ~moveS(ep_squares[1][pos->ep]);
							pos->bpieces &= ~moveS(ep_squares[1][pos->ep]);
							pos->hash ^= Hash[bpawns_n][mv->to - 8];
							pos->limp1 = pos->move_num + 1;
							pos->half_move_clock = -1;
							break;
						default:    //capturing white
							pos->score -= PAWN_VAL;
							pos->tot_mat -= PAWN_VAL;
							pos->wpawns &= ~moveN(ep_squares[0][pos->ep]);
							pos->wpieces &= ~moveN(ep_squares[0][pos->ep]);
							pos->hash ^= Hash[wpawns_n][mv->to + 8];
							pos->limp1 = pos->move_num + 1;
							pos->half_move_clock = -1;
							break;
					}
					break;
			}
			// for both promotion and ordinary moves, piece moves from its old square, and sumpieces moves from old square to new square.
			// for ordinary moves, the piece moves to its new square, but for promotion moves, the pawn is removed and a new piece substituted
			pos->pieces[mv->from_p] &= notlinboard[mv->from];
			pos->hash ^= Hash[mv->from_p][mv->from];

			pos->sumpieces[mv->from_p & COLOR] |= linboard[mv->to];
			pos->sumpieces[mv->from_p & COLOR] &= notlinboard[mv->from];

			switch(mv->from_p) // to change castling permissions and limp1. no change of limp1 on king or rook moves because they don't NECESSARILY change castling permissions. Not changing limp1 when there is a change in castling permissions does not affect the results of is_rep and probably has only a very small detrimental effect on perfomance
			{
				case wpawns_n:
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case bpawns_n:
					pos->limp1 = pos->move_num + 1;
					pos->half_move_clock = -1;
					break;
				case wking_n: 
					pos->hash ^= CastlingHash[pos->perm_castling];
					pos->perm_castling |= (WKING | WQUEEN);
					pos->hash ^= CastlingHash[pos->perm_castling];
					break;
				case bking_n: 
					pos->hash ^= CastlingHash[pos->perm_castling];
					pos->perm_castling |= (BKING | BQUEEN);
					pos->hash ^= CastlingHash[pos->perm_castling];
					break;
				case wrooks_n:
					switch(mv->from)
					{
						case 0:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= WQUEEN;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						case 7:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= WKING;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						default:
							break;
					}
					break;
				case brooks_n:
					switch(mv->from)
					{
						case 56:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= BQUEEN;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						case 63:
							pos->hash ^= CastlingHash[pos->perm_castling];
							pos->perm_castling |= BKING;
							pos->hash ^= CastlingHash[pos->perm_castling];
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}

			switch(mv->special)
			{
				case EMPTY:
					pos->pieces[mv->from_p] |= linboard[mv->to];
					pos->hash ^= Hash[mv->from_p][mv->to];

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case ADVANCE2:
					pos->pieces[mv->from_p] |= linboard[mv->to];
					pos->hash ^= Hash[mv->from_p][mv->to];
					switch(pos->tomove)
					{
						case WHITE:
							pos->hash ^= EPHash[pos->ep];
							pos->ep = mv->from & FILEMASK;
							pos->hash ^= EPHash[pos->ep];
							break;
						case BLACK:
							pos->hash ^= EPHash[pos->ep];
							pos->ep = mv->from & FILEMASK;
							pos->hash ^= EPHash[pos->ep];
							break;
					}
					break;
				case(QUEEN | WHITE):
					pos->wqueens |= linboard[mv->to];
					pos->hash ^= WQueensHash[mv->to];

					pos->score += (QUEEN_VAL - PAWN_VAL);
					pos->tot_mat += (QUEEN_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(QUEEN | BLACK):
					pos->bqueens |= linboard[mv->to];
					pos->hash ^= BQueensHash[mv->to];

					pos->score -= (QUEEN_VAL - PAWN_VAL);
					pos->tot_mat += (QUEEN_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(KNIGHT | WHITE):
					pos->wknights |= linboard[mv->to];
					pos->hash ^= WKnightsHash[mv->to];

					pos->score += (KNIGHT_VAL - PAWN_VAL);
					pos->tot_mat += (KNIGHT_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(KNIGHT | BLACK):
					pos->bknights |= linboard[mv->to];
					pos->hash ^= BKnightsHash[mv->to];

					pos->score -= (KNIGHT_VAL - PAWN_VAL);
					pos->tot_mat += (KNIGHT_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(ROOK | WHITE):
					pos->wrooks |= linboard[mv->to];
					pos->hash ^= WRooksHash[mv->to];

					pos->score += (ROOK_VAL - PAWN_VAL);
					pos->tot_mat += (ROOK_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(ROOK | BLACK):
					pos->brooks |= linboard[mv->to];
					pos->hash ^= BRooksHash[mv->to];

					pos->score -= (ROOK_VAL - PAWN_VAL);
					pos->tot_mat += (ROOK_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(BISHOP | WHITE):
					pos->wbishops |= linboard[mv->to];
					pos->hash ^= WBishopsHash[mv->to];

					pos->score += (BISHOP_VAL - PAWN_VAL);
					pos->tot_mat += (BISHOP_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
				case(BISHOP | BLACK):
					pos->bbishops |= linboard[mv->to];
					pos->hash ^= BBishopsHash[mv->to];

					pos->score -= (BISHOP_VAL - PAWN_VAL);
					pos->tot_mat += (BISHOP_VAL - PAWN_VAL);

					pos->hash ^= EPHash[pos->ep];
					pos->ep = EP_NONE;
					pos->hash ^= EPHash[pos->ep];
					break;
			}
			break; //end no castling

		// upon castling, we always change limp1 (because castling permissions are guaranteed to change). This isn't exactly in line with FIDE rules, as detailed in history.c, but it does make our code run faster. If we change our program to exactly implement the FIDE 3-fold repetition rule, we would have to think about changing this.
		case WKING:
			pos->wking = board[0][6];
			pos->wrooks = board[0][5] | (~board[0][7] & pos->wrooks);

			pos->wpieces |= board[0][6];
			pos->hash ^= WKingHash[6];
			pos->wpieces &= ~board[0][4];
			pos->hash ^= WKingHash[4];

			pos->wpieces |= board[0][5];
			pos->hash ^= WRooksHash[5];
			pos->wpieces &= ~board[0][7];
			pos->hash ^= WRooksHash[7];

			pos->hash ^= CastlingHash[pos->perm_castling];
			pos->perm_castling |= (WKING | WQUEEN);
			pos->hash ^= CastlingHash[pos->perm_castling];

			pos->score += CASTLING_BONUS;

			pos->hash ^= EPHash[pos->ep];
			pos->ep = EP_NONE;
			pos->hash ^= EPHash[pos->ep];

			pos->limp1 = pos->move_num + 1;
			break;
		case WQUEEN:
			pos->wking = board[0][2];
			pos->wrooks = board[0][3] | (~board[0][0] & pos->wrooks);

			pos->wpieces |= board[0][2];
			pos->hash ^= WKingHash[2];
			pos->wpieces &= ~board[0][4];
			pos->hash ^= WKingHash[4];

			pos->wpieces |= board[0][3];
			pos->hash ^= WRooksHash[3];
			pos->wpieces &= ~board[0][0];
			pos->hash ^= WRooksHash[0];

			pos->hash ^= CastlingHash[pos->perm_castling];
			pos->perm_castling |= (WKING | WQUEEN);
			pos->hash ^= CastlingHash[pos->perm_castling];

			pos->score += CASTLING_BONUS;

			pos->hash ^= EPHash[pos->ep];
			pos->ep = EP_NONE;
			pos->hash ^= EPHash[pos->ep];

			pos->limp1 = pos->move_num + 1;
			break;
		case BKING:
			pos->bking = board[7][6];
			pos->brooks = board[7][5] | (~board[7][7] & pos->brooks);

			pos->bpieces |= board[7][6];
			pos->hash ^= BKingHash[7*8 + 6];
			pos->bpieces &= ~board[7][4];
			pos->hash ^= BKingHash[7*8 + 4];

			pos->bpieces |= board[7][5];
			pos->hash ^= BRooksHash[7*8 + 5];
			pos->bpieces &= ~board[7][7];
			pos->hash ^= BRooksHash[7*8 + 7];

			pos->hash ^= CastlingHash[pos->perm_castling];
			pos->perm_castling |= (BKING | BQUEEN);
			pos->hash ^= CastlingHash[pos->perm_castling];

			pos->score -= CASTLING_BONUS;

			pos->hash ^= EPHash[pos->ep];
			pos->ep = EP_NONE;
			pos->hash ^= EPHash[pos->ep];

			pos->limp1 = pos->move_num + 1;
			break;
		case BQUEEN:
			pos->bking = board[7][2];
			pos->brooks = board[7][3] | (~board[7][0] & pos->brooks);

			pos->bpieces |= board[7][2];
			pos->hash ^= BKingHash[7*8 + 2];
			pos->bpieces &= ~board[7][4];
			pos->hash ^= BKingHash[7*8 + 4];

			pos->bpieces |= board[7][3];
			pos->hash ^= BRooksHash[7*8 + 3];
			pos->bpieces &= ~board[7][0];
			pos->hash ^= BRooksHash[7*8 + 0];

			pos->hash ^= CastlingHash[pos->perm_castling];
			pos->perm_castling |= (BKING | BQUEEN);
			pos->hash ^= CastlingHash[pos->perm_castling];

			pos->score -= CASTLING_BONUS;

			pos->hash ^= EPHash[pos->ep];
			pos->ep = EP_NONE;
			pos->hash ^= EPHash[pos->ep];

			pos->limp1 = pos->move_num + 1;
			break;
	}

	// switch tomove
	pos->allpieces = pos->wpieces | pos->bpieces;

	pos->hash ^= ToMoveHash[pos->tomove];
	unsigned char tmp = pos->tomove;
	pos->tomove = pos->towait;
	pos->towait = tmp;
	pos->hash ^= ToMoveHash[pos->tomove];

	pos->move_num++;
	pos->half_move_clock++;

	#ifdef _USE_HISTORY
	hist->keys[pos->move_num] = pos->hash;
	#endif

	assert(consistency(pos));
}

/*
// undo move
inline void make_unmove(struct position *pos, struct move *mv)
{
	assert(mv->from < 64);
	assert(mv->to < 64);
	switch(mv->castling)
	{
		case NONE:
			position->pieces[mv->from_p] |= linboard[mv->from];
			position->pieces[mv->from_p] &= notlinboard[mv->to];

			pos->sumpieces[mv->from_p & COLOR] |= linboard[mv->from];
			pos->sumpieces[mv->from_p & COLOR] &= notlinboard[mv->to];
			switch(mv->to_p)
			{
				case nopiece_n:
					break;
				default:
			}
	}
}
*/

#endif
