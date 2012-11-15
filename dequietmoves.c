// dequietmoves.c
// move generation for quiet(.)
// generates captures, pawn promotions, and (later) checks

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _DEBUG
#define NDEBUG 
#endif
#include <assert.h>

#include "minmax.h"
#include "position.h"
#include "move.h"
#include "addmove.c"

void rook_move_dq(struct position *, struct move_array *, unsigned char);
inline void rook_move_dq(struct position *pos, struct move_array *m, unsigned char rooks)
{
	unsigned char opponent = -(rooks & COLOR) + 1;
	uint64_t rook_pos = pos->pieces[rooks];
	uint64_t moves;
	unsigned char index_from, index_to;
	for(int i = 0; i < 10; i++){ // up to 10 rooks of same color on the board, (for loop for compiler loop unrolling)
		if((index_from = ffsll(rook_pos)) != 0){
			index_from--;
			moves = RookMoves[index_from][magictransform( (pos->allpieces & RookMasks[index_from]), RookMagic[index_from], RookMagicSize[index_from] )] & pos->sumpieces[opponent];
			for(int i = 0; i < 14; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_from, index_to, rooks, find_piece(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}
			rook_pos &= notlinboard[index_from];
		}
		else {
			break;
		}
	}
}

void bishop_move_dq(struct position *, struct move_array *, unsigned char);
inline void bishop_move_dq(struct position *pos, struct move_array *m, unsigned char bishops)
{
	unsigned char opponent = -(bishops & COLOR) + 1;
	uint64_t bishop_pos = pos->pieces[bishops];
	uint64_t moves;
	unsigned char index_from, index_to;
	for(int i = 0; i < 10; i++){ // up to 10 bishops of same color on the board, (for loop for compiler loop unrolling)
		if((index_from = ffsll(bishop_pos)) != 0){
			index_from--;
			moves = BishopMoves[index_from][magictransform( (pos->allpieces & BishopMasks[index_from]), BishopMagic[index_from], BishopMagicSize[index_from] )] & pos->sumpieces[opponent];
			for(int i = 0; i < 14; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_from, index_to, bishops, find_piece(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}
			bishop_pos &= notlinboard[index_from];
		}
		else {
			break;
		}
	}
}

void knight_move_dq(struct position *, struct move_array *, unsigned char);
inline void knight_move_dq(struct position *pos, struct move_array *m, unsigned char knights)
{
	unsigned char opponent = -(knights & COLOR) + 1;
	uint64_t knight_pos = pos->pieces[knights];
	uint64_t moves;
	unsigned char index_from, index_to;
	for(int i = 0; i < 10; i++){ // can have up to 10 knights of same color on the board, (for loop for compiler loop unrolling)
		if((index_from = ffsll(knight_pos)) != 0){
			index_from--;
			moves = knightmoves[index_from] & pos->sumpieces[opponent];
			for(int i = 0; i < 8; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_from, index_to, knights, find_piece(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}
			knight_pos &= notlinboard[index_from];
		}
		else {
			break;
		}
	}
}

void king_move_dq(struct position *, struct move_array *, unsigned char);
inline void king_move_dq(struct position *pos, struct move_array *m, unsigned char king)
{
	unsigned char opponent = -(king & COLOR) + 1;
	unsigned char from = ffsll(pos->pieces[king]) - 1;
	uint64_t moves = kingmoves[from] & pos->sumpieces[opponent];
	
	unsigned char index;
	for(int i = 0; i < 8; i++){
		if((index = ffsll(moves)) != 0){
			index--;
			add_move(m, from, index, king, find_piece(pos, index));
			moves &= notlinboard[index];
		}
		else {
			break;
		}
	}
}

void pawn_move_dq(struct position *, struct move_array *, unsigned char);
inline void pawn_move_dq(struct position *pos, struct move_array *m, unsigned char pawns)
{
	uint64_t pawn_pos = pos->pieces[pawns];
	uint64_t moves;
	unsigned char index_to;
	switch(pawns & COLOR)
	{
		case WHITE:	
			// nonpromotion attack west moves
			pawn_pos = pos->pieces[pawns];
			moves = moveNW(pawn_pos) & (pos->bpieces | ep_squares[1][pos->ep]) & ~file[7] & ~rank[7];
			for(int i = 0; i < 8; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_to + SE, index_to, pawns, find_piece_ep(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}

			// nonpromotion attack east moves
			pawn_pos = pos->pieces[pawns];
			moves = moveNE(pawn_pos) & (pos->bpieces | ep_squares[1][pos->ep]) & ~file[0] & ~rank[7];
			for(int i = 0; i < 8; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_to + SW, index_to, pawns, find_piece_ep(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}

			pawn_pos = pos->pieces[pawns];
			if((pawn_pos & rank[6]) == 0){ // no promotion possibilities
				return;
			}
			else {
				// promotion forward moves
				moves = moveN(pawn_pos) & ~pos->allpieces & rank[7];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + S, index_to, pawns, nopiece_n);
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}
			
				// promotion attack west moves
				pawn_pos = pos->pieces[pawns];
				moves = moveNW(pawn_pos) & pos->bpieces & ~file[7] & rank[7];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + SE, index_to, pawns, find_piece(pos, index_to));
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}

				// promotion attack east moves
				pawn_pos = pos->pieces[pawns];
				moves = moveNE(pawn_pos) & pos->bpieces & ~file[0] & rank[7];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + SW, index_to, pawns, find_piece(pos, index_to));
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}
			}
			break;
		default: //black
			// nonpromotion attack west moves
			pawn_pos = pos->pieces[pawns];
			moves = moveSW(pawn_pos) & (pos->wpieces | ep_squares[0][pos->ep]) & ~file[7] & ~rank[0];
			for(int i = 0; i < 8; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_to + NE, index_to, pawns, find_piece_ep(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}

			// nonpromotion attack east moves
			pawn_pos = pos->pieces[pawns];
			moves = moveSE(pawn_pos) & (pos->wpieces | ep_squares[0][pos->ep]) & ~file[0] & ~rank[0];
			for(int i = 0; i < 8; i++){
				if((index_to = ffsll(moves)) != 0){
					index_to--;
					add_move(m, index_to + NW, index_to, pawns, find_piece_ep(pos, index_to));
					moves &= notlinboard[index_to];
				}
				else {
					break;
				}
			}

			pawn_pos = pos->pieces[pawns];
			if((pawn_pos & rank[1]) == 0){ // no promotion possibilities
				return;
			}
			else {
				// promotion forward moves
				moves = moveS(pawn_pos) & ~pos->allpieces & rank[0];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + N, index_to, pawns, nopiece_n);
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}
			
				// promotion attack west moves
				pawn_pos = pos->pieces[pawns];
				moves = moveSW(pawn_pos) & pos->wpieces & ~file[7] & rank[0];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + NE, index_to, pawns, find_piece(pos, index_to));
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}

				// promotion attack east moves
				pawn_pos = pos->pieces[pawns];
				moves = moveSE(pawn_pos) & pos->wpieces & ~file[0] & rank[0];
				for(int i = 0; i < 8; i++){
					if((index_to = ffsll(moves)) != 0){
						index_to--;
						add_promotion_move(m, index_to + NW, index_to, pawns, find_piece(pos, index_to));
						moves &= notlinboard[index_to];
					}
					else {
						break;
					}
				}
			}
			break;
	}
}


/*
void castling_move(struct position *, struct move_array *, unsigned char);
inline void castling_move(struct position *pos, struct move_array *m, unsigned char king)
{
	switch(king & COLOR)
	{
		case WHITE:
			if(!(pos->perm_castling & WKING)){ // look for attacks on 4,5,6
				if(!(pos->allpieces & Piece_Castling[PC_WKING])){ // no obstructing pieces
					if(!(pos->bknights & Knight_Castling[KC_WKING])){ // no knight attacks
						if(!((pos->bpawns | pos->bking) & Pawn_Castling[PC_WKING])){ // no pawn or king attacks
							int ok = 1;
							for(int j = 4; j <= 6; j++){ // no queen, rook, or bishop attacks
								if(BishopMoves[0*8 + j][magictransform( (pos->allpieces & BishopMasks[0*8 + j]), BishopMagic[0*8 + j], BishopMagicSize[0*8 + j] )] & (pos->bbishops | pos->bqueens)){ // pretend squares are bishops, and see if they can attack an enemy bishop or queen
									ok = 0;
									break;
								}
								if(RookMoves[0*8 + j][magictransform( (pos->allpieces & RookMasks[0*8 + j]), RookMagic[0*8 + j], RookMagicSize[0*8 + j] )] & (pos->brooks | pos->bqueens)){ // pretend squares are rooks, and see if they can attack an enemy rook or queen
									ok = 0;
									break;
								}
							}
							if(ok){
								add_castling_move(m, WKING);
							}
						}
					}
				}
			}
			if(!(pos->perm_castling & WQUEEN)){ // look for attacks on 2,3,4
				if(!(pos->allpieces & Piece_Castling[PC_WQUEEN])){ // no obstructing pieces
					if(!(pos->bknights & Knight_Castling[KC_WQUEEN])){ // no knight attacks
						if(!((pos->bpawns | pos->bking) & Pawn_Castling[PC_WQUEEN])){ // no pawn or king attacks
							int ok = 1;
							for(int j = 2; j <= 4; j++){ // no queen, rook, or bishop attacks
								if(BishopMoves[0*8 + j][magictransform( (pos->allpieces & BishopMasks[0*8 + j]), BishopMagic[0*8 + j], BishopMagicSize[0*8 + j] )] & (pos->bbishops | pos->bqueens)){ // pretend squares are bishops, and see if they can attack an enemy bishop or queen
									ok = 0;
									break;
								}
								if(RookMoves[0*8 + j][magictransform( (pos->allpieces & RookMasks[0*8 + j]), RookMagic[0*8 + j], RookMagicSize[0*8 + j] )] & (pos->brooks | pos->bqueens)){ // pretend squares are rooks, and see if they can attack an enemy rook or queen
									ok = 0;
									break;
								}
							}
							if(ok){
								add_castling_move(m, WQUEEN);
							}
						}
					}
				}
			}
			break;
		default:
			if(!(pos->perm_castling & BKING)){ // look for attacks on 4,5,6
				if(!(pos->allpieces & Piece_Castling[PC_BKING])){ // no obstructing pieces
					if(!(pos->wknights & Knight_Castling[KC_BKING])){ // no knight attacks
						if(!((pos->wpawns | pos->wking) & Pawn_Castling[PC_BKING])){ // no pawn or king attacks
							int ok = 1;
							for(int j = 4; j <= 6; j++){ // no queen, rook, or bishop attacks
								if(BishopMoves[7*8 + j][magictransform( (pos->allpieces & BishopMasks[7*8 + j]), BishopMagic[7*8 + j], BishopMagicSize[7*8 + j] )] & (pos->wbishops | pos->wqueens)){ // pretend squares are bishops, and see if they can attack an enemy bishop or queen
									ok = 0;
									break;
								}
								if(RookMoves[7*8 + j][magictransform( (pos->allpieces & RookMasks[7*8 + j]), RookMagic[7*8 + j], RookMagicSize[7*8 + j] )] & (pos->wrooks | pos->wqueens)){ // pretend squares are rooks, and see if they can attack an enemy rook or queen
									ok = 0;
									break;
								}
							}
							if(ok){
								add_castling_move(m, BKING);
							}
						}
					}
				}
			}
			if(!(pos->perm_castling & BQUEEN)){ // look for attacks on 2,3,4
				if(!(pos->allpieces & Piece_Castling[PC_BQUEEN])){ // no obstructing pieces
					if(!(pos->wknights & Knight_Castling[KC_BQUEEN])){ // no knight attacks
						if(!((pos->wpawns | pos->wking) & Pawn_Castling[PC_BQUEEN])){ // no pawn or king attacks
							int ok = 1;
							for(int j = 2; j <= 4; j++){ // no queen, rook, or bishop attacks
								if(BishopMoves[7*8 + j][magictransform( (pos->allpieces & BishopMasks[7*8 + j]), BishopMagic[7*8 + j], BishopMagicSize[7*8 + j] )] & (pos->wbishops | pos->wqueens)){ // pretend squares are bishops, and see if they can attack an enemy bishop or queen
									ok = 0;
									break;
								}
								if(RookMoves[7*8 + j][magictransform( (pos->allpieces & RookMasks[7*8 + j]), RookMagic[7*8 + j], RookMagicSize[7*8 + j] )] & (pos->wrooks | pos->wqueens)){ // pretend squares are rooks, and see if they can attack an enemy rook or queen
									ok = 0;
									break;
								}
							}
							if(ok){
								add_castling_move(m, BQUEEN);
							}
						}
					}
				}
			}
	}
}
*/

// return array of all possible moves
struct move_array *dequietmoves(struct position *);
inline struct move_array *dequietmoves(struct position *pos)
{
	// change the move ordering !!!
	struct move_array *m = new_move_array();

	switch(pos->tomove)
	{
		case WHITE:
			rook_move_dq(pos, m, wqueens_n);
			bishop_move_dq(pos, m, wqueens_n);
			bishop_move_dq(pos, m, wbishops_n);
			knight_move_dq(pos, m, wknights_n);
			rook_move_dq(pos, m, wrooks_n);
			pawn_move_dq(pos, m, wpawns_n);
			king_move_dq(pos, m, wking_n);
			
			//castling_move(pos, m, wking_n);
			return(m);
						
		default:
			rook_move_dq(pos, m, bqueens_n);
			bishop_move_dq(pos, m, bqueens_n);
			bishop_move_dq(pos, m, bbishops_n);
			knight_move_dq(pos, m, bknights_n);
			rook_move_dq(pos, m, brooks_n);
			pawn_move_dq(pos, m, bpawns_n);
			king_move_dq(pos, m, bking_n);

			//castling_move(pos, m, bking_n);
			break;
	}
	return(m);
}
