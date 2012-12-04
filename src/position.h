// position.h
// structs to represent positions

#ifndef _POSITION_H
#define _POSITION_H

#include <stdint.h>

#include "params.h"

#define WHITE   0x00
#define BLACK   0x01 //use bitwise or

#define COLOR   0x01

// for special
#define QUEEN   0x02 
#define ROOK    0x04 
#define BISHOP  0x06 
#define KNIGHT  0x08 
#define PROMOMASK 14
#define ADVANCE2  16

// defs for permitted castling; if(castling & flag), then flag not permitted
#define ALL 0x00
#define WKING   0x01
#define BKING   0x02
#define WQUEEN  0x04
#define BQUEEN  0x08
#define NONE    0x0F

#define KING_CAPTURED_SCORE (800 * PAWN_VAL)

#define KING_VAL    piece_vals[0]
#define QUEEN_VAL   piece_vals[2]
#define ROOK_VAL    piece_vals[4]
#define BISHOP_VAL  piece_vals[6]
#define KNIGHT_VAL  piece_vals[8]

#include "bitboard.h"
#include "movegen/kingmove.h"
#include "movegen/knightmove.h"
#include "movegen/file.h"
#include "movegen/rank.h"
#include "movegen/slidingmoves.h"
#include "movegen/knight_castling.h"
#include "movegen/pawn_castling.h"
#include "movegen/piece_castling.h"
#include "movegen/pawn_attacks.h"
#include "movegen/ep_squares.h"
#include "movegen/vector.h"

// for bitboards (uint64_t)
#define moveN(x)  ((x) << 8 )
#define moveNE(x) ((x) << 9 )
#define moveE(x)  ((x) << 1 )
#define moveSE(x) ((x) >> 7 )
#define moveS(x)  ((x) >> 8 )
#define moveSW(x) ((x) >> 9 )
#define moveW(x)  ((x) >> 1 )
#define moveNW(x) ((x) << 7 )

// for indices
#define rank(i)   ((i) >> 3 )
#define file(i)   ((i) & 0x7)

/* flip the board, for example with:
0 56
1 57
2 58
3 59
4 60
5 61
6 62
7 63
8 48
*/
#define flip(i)   (file(i) + 8 * (7 - rank(i)))

#define N  (8)
#define NE (9)
#define E  (1)
#define SE (-7)
#define S  (-8)
#define SW (-9)
#define W  (-1)
#define NW (7)

#define EMPTY   0x0000000000000000

struct position
{
    unsigned char   tomove;
    unsigned char   towait;
    unsigned char   perm_castling; // for now and ever
    unsigned char   ep; //en passant file. value of 8 means no ep square
    #define EP_NONE 8

    uint64_t    pieces[12];

    // sorted in order of value for use in move sorting
    #define wking_n     0
    #define bking_n     1
    #define wqueens_n   2
    #define bqueens_n   3
    #define wrooks_n    4
    #define brooks_n    5
    #define wbishops_n  6
    #define bbishops_n  7
    #define wknights_n  8
    #define bknights_n  9
    #define wpawns_n    10
    #define bpawns_n    11
    #define eppiece_n   12
    #define nopiece_n   13 // captures evaluated first

    #define wking       pieces[0]
    #define bking       pieces[1]
    #define wqueens     pieces[2]
    #define bqueens     pieces[3]
    #define wrooks      pieces[4]
    #define brooks      pieces[5]
    #define wbishops    pieces[6]
    #define bbishops    pieces[7]
    #define wknights    pieces[8]
    #define bknights    pieces[9]
    #define wpawns      pieces[10]
    #define bpawns      pieces[11]

    uint64_t    sumpieces[2];
    #define wpieces     sumpieces[0]
    #define bpieces     sumpieces[1]
    /*
    uint64_t    antipieces[2]; // = ~sumpieces, ie, where there are no black/white pieces
    #define notwpieces  antipieces[0]
    #define notbpieces  antipieces[1]
    */
    uint64_t    allpieces;  // = wpieces | bpieces = all the pieces on the board
    uint64_t    hash;       //hash of this position
    int32_t     score;      //material balance
    int32_t     tot_mat;    //total material on board, (except kings)
    uint16_t    move_num;   //number of half moves so far.
    uint16_t    limp1;      //"Last Irreversible Move Plus 1", the move_num of the position following the last irreversible move.
    signed char half_move_clock;// number of half moves since the last pawn move or capture. Important that the char be signed, so that can include -1 in makemove.c
}; 

void print_position(struct position*);
void init_position(struct position*);
char data_to_human(struct position*, unsigned char, unsigned char);

#endif
