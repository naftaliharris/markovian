// countmoves.c
// exactly the same as allmoves.c, but count instead of generate

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

int rook_move_count(struct position *, unsigned char);
inline int rook_move_count(struct position *pos, unsigned char rooks)
{
    int count = 0;
    uint64_t rook_pos = pos->pieces[rooks];
    uint64_t moves;
    unsigned char index_from, index_to;
    for (int i = 0; i < 10; i++) {  // up to 10 rooks of same color on the board, (for loop for compiler loop unrolling)
        if ((index_from = ffsll(rook_pos)) != 0) {
            index_from--;
            moves = RookMoves[index_from][magictransform((pos->allpieces & RookMasks[index_from]), RookMagic[index_from], RookMagicSize[index_from])] & ~pos->sumpieces[rooks & COLOR];
            for (int i = 0; i < 14; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count++;
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
    return (count);
}

int bishop_move_count(struct position *, unsigned char);
inline int bishop_move_count(struct position *pos, unsigned char bishops)
{
    int count = 0;
    uint64_t bishop_pos = pos->pieces[bishops];
    uint64_t moves;
    unsigned char index_from, index_to;
    for (int i = 0; i < 10; i++) {  // up to 10 bishops of same color on the board, (for loop for compiler loop unrolling)
        if ((index_from = ffsll(bishop_pos)) != 0) {
            index_from--;
            moves = BishopMoves[index_from][magictransform ((pos->allpieces & BishopMasks[index_from]), BishopMagic[index_from], BishopMagicSize [index_from])] & ~pos->sumpieces[bishops & COLOR];
            for (int i = 0; i < 14; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count++;
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
    return (count);
}

int knight_move_count(struct position *, unsigned char);
inline int knight_move_count(struct position *pos, unsigned char knights)
{
    int count = 0;
    uint64_t knight_pos = pos->pieces[knights];
    uint64_t moves;
    unsigned char index_from, index_to;
    for (int i = 0; i < 10; i++) {  // can have up to 10 knights of same color on the board, (for loop for compiler loop unrolling)
        if ((index_from = ffsll(knight_pos)) != 0) {
            index_from--;
            moves = knightmoves[index_from] & ~pos->sumpieces[knights & COLOR];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count++;
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
    return (count);
}

int king_move_count(struct position *, unsigned char);
inline int king_move_count(struct position *pos, unsigned char king)
{
    int count = 0;
    unsigned char from = ffsll(pos->pieces[king]) - 1;
    uint64_t moves = kingmoves[from] & ~pos->sumpieces[king & COLOR];

    unsigned char index;
    for (int i = 0; i < 8; i++) {
        if ((index = ffsll(moves)) != 0) {
            index--;
            count++;
            moves &= notlinboard[index];
        } else {
            break;
        }
    }
    return (count);
}

int pawn_move_count(struct position *, unsigned char);
inline int pawn_move_count(struct position *pos, unsigned char pawns)
{
    int count = 0;
    uint64_t pawn_pos = pos->pieces[pawns];
    uint64_t moves;
    unsigned char index_to;
    switch (pawns & COLOR) {
    case WHITE:
        // nonpromotion forward moves 
        moves = moveN(pawn_pos) & ~pos->allpieces & ~rank[7];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // forward 2 moves 
        pawn_pos = pos->pieces[pawns];
        moves = moveN(moveN(pawn_pos) & ~pos->allpieces) & ~pos->allpieces & rank[3];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // nonpromotion attack west moves
        pawn_pos = pos->pieces[pawns];
        moves = moveNW(pawn_pos) & pos->bpieces & ~file[7] & ~rank[7];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // nonpromotion attack east moves
        pawn_pos = pos->pieces[pawns];
        moves = moveNE(pawn_pos) & pos->bpieces & ~file[0] & ~rank[7];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        pawn_pos = pos->pieces[pawns];
        if ((pawn_pos & rank[6]) != 0) {    // no promotion possibilities
            return (count);
        } else {
            // promotion forward moves
            moves = moveN(pawn_pos) & ~pos->allpieces & rank[7];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }

            // promotion attack west moves
            pawn_pos = pos->pieces[pawns];
            moves = moveNW(pawn_pos) & pos->bpieces & ~file[7] & rank[7];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }

            // promotion attack east moves
            pawn_pos = pos->pieces[pawns];
            moves = moveNE(pawn_pos) & pos->bpieces & ~file[0] & rank[7];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }
        }
        break;
    default:        //black
        // nonpromotion forward moves 
        moves = moveS(pawn_pos) & ~pos->allpieces & ~rank[0];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // forward 2 moves 
        pawn_pos = pos->pieces[pawns];
        moves = moveS(moveS(pawn_pos) & ~pos->allpieces) & ~pos->allpieces & rank[4];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // nonpromotion attack west moves
        pawn_pos = pos->pieces[pawns];
        moves = moveSW(pawn_pos) & pos->wpieces & ~file[7] & ~rank[0];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        // nonpromotion attack east moves
        pawn_pos = pos->pieces[pawns];
        moves = moveSE(pawn_pos) & pos->wpieces & ~file[0] & ~rank[0];
        for (int i = 0; i < 8; i++) {
            if ((index_to = ffsll(moves)) != 0) {
                index_to--;
                count++;
                moves &= notlinboard[index_to];
            } else {
                break;
            }
        }

        pawn_pos = pos->pieces[pawns];
        if ((pawn_pos & rank[1]) != 0) {    // no promotion possibilities
            return (count);
        } else {
            // promotion forward moves
            moves = moveS(pawn_pos) & ~pos->allpieces & rank[0];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }

            // promotion attack west moves
            pawn_pos = pos->pieces[pawns];
            moves = moveSW(pawn_pos) & pos->wpieces & ~file[7] & rank[0];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }

            // promotion attack east moves
            pawn_pos = pos->pieces[pawns];
            moves = moveSE(pawn_pos) & pos->wpieces & ~file[0] & rank[0];
            for (int i = 0; i < 8; i++) {
                if ((index_to = ffsll(moves)) != 0) {
                    index_to--;
                    count += 4;
                    moves &= notlinboard[index_to];
                } else {
                    break;
                }
            }
        }
        break;
    }
    return (count);
}

// count of all possible moves
int countmoves(struct position *);
inline int countmoves(struct position *pos)
{
    int count = 0;
    switch (pos->tomove) {
    case WHITE:
        count += rook_move_count(pos, wqueens_n);
        count += bishop_move_count(pos, wqueens_n);
        count += bishop_move_count(pos, wbishops_n);
        count += knight_move_count(pos, wknights_n);
        count += rook_move_count(pos, wrooks_n);
        count += pawn_move_count(pos, wpawns_n);
        count += king_move_count(pos, wking_n);
        break;
    default:
        count += rook_move_count(pos, bqueens_n);
        count += bishop_move_count(pos, bqueens_n);
        count += bishop_move_count(pos, bbishops_n);
        count += knight_move_count(pos, bknights_n);
        count += rook_move_count(pos, brooks_n);
        count += pawn_move_count(pos, bpawns_n);
        count += king_move_count(pos, bking_n);
        break;
    }
    return (count);
}
