// hash.h
// structs for hashing positions

#ifndef _HASH_H
#define _HASH_H

#include "hashrands.h"
#include "../move.h"

#define TRANS_LOOKUP_BITS 25 //for the transposition tables

struct hashed
{
    uint64_t hash; //need at least 5 bytes for this, (4 means 4 lookup bytes = 4Giga transposition table), but unclear whether entire 8 bytes is any faster.
    int32_t score;
    struct move bestmove;
    unsigned char flags;
    #define HASH_EXACT 0 // this is an exact number
    #define HASH_LB    1 // this is a lower bound
    #define HASH_EXACT_MASK 1
    #define HASH_SCORE 0 // (evaluated in score, all moves considered)
    #define HASH_QUIET 2 // (evaluated in quiet, quieting moves considered)
    #define HASH_SCORE_MASK 2
    // Governing principle: Lower numbers are generally better.
    /*  Flags   Meaning
        0   Exact score
        1   Lowerbound score
        2   Exact quiet
        3   Lowerbound quiet
    */

    uint16_t move_num;  //number of half moves made to arrive at this position. Upon not matching, this is all we need to know to decide whether to overwrite.
    char ply;       //how far result is from the bottom. Upon a match, this is all we need to know to decide whether to recompute.
    #define HASH_QUIET_PLY -127 //will always be removed
};

struct hashed *tt; //transposition table

#endif
