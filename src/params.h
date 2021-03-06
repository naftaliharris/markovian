/* Section: Header */
// params.h
// parameters used by Markovian

/******************************************************************
 * This file is edited and generated by ../learn/params.py, so be *
 * careful to observe the format outlined there                   *
 ******************************************************************/

#ifndef _PARAMS_H
#define _PARAMS_H

/* Section: Global Definitions */
// important: pawn_val must be at _LEAST_ this big for tolerance/100 to be positive
// also can't be much bigger, else infinity = 10000 * pawn_val will overflow
const int32_t PAWN_VAL = 100000;
const int32_t CENTIPAWN = 1000;

const int32_t MATCUTOFF = 150000;

/* Section: Search Parameters */
const int32_t to_p_vals[14] = {100000000, 100000000, 900000, 900000, 500000, 500000, 325000, 325000, 300000, 300000, 100000, 100000, 100000, -1000000};

const int32_t from_p_vals[14] = {-100000000, -100000000, 900000, 900000, 500000, 500000, 325000, 325000, 300000, 300000, 100000, 100000, 100000, -1000000};

const int32_t ASPIRATION_WINDOW = 25000;

/* Section: Strategic Parameters */

// this is always a positive quantity
const int32_t CONTEMPT = 0;
const float COUNTVAL = 0.01;
const int32_t CASTLING_BONUS = 25000;

const int32_t piece_vals[14] = {100000000, 100000000, 900000, 900000, 500000, 500000, 325000, 325000, 300000, 300000, 100000, 100000, 100000, 0};

const uint32_t control_params[64] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1500, 1500, 1500, 1500, 1500, 1500, 1000, 1000, 1500, 2000, 2000, 2000, 2000, 1500, 1000, 1000, 1500, 2000, 3000, 3000, 2000, 1500, 1000, 1000, 1500, 2000, 3000, 3000, 2000, 1500, 1000, 1000, 1500, 2000, 2000, 2000, 2000, 1500, 1000, 1000, 1500, 1500, 1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};

// penalties per rank for having doubled pawns
const uint32_t doubled_penalty[8] = {-5000, -10000, -15000, -20000, -20000, -15000, -10000, -5000};

const uint32_t WPassed_points[8] = {0, 20000, 25000, 35000, 50000, 65000, 110000, 0};

const uint32_t BPassed_points[8] = {0, 110000, 65000, 50000, 35000, 25000, 20000, 0};

/* Section: Footer */
#endif
