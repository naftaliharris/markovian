// params.h
// parameters used by Markovian

#ifndef _PARAMS_H
#define _PARAMS_H

/* Global Definitions */
// important: pawn_val must be at _LEAST_ this big for tolerance/100 to be positive
// also can't be much bigger, else infinity = 10000 * pawn_val will overflow
#define PAWN_VAL 100000
#define CENTIPAWN 1000

/* Search Parameters */
const int32_t to_p_vals[14] = {
	1000 * PAWN_VAL,
	1000 * PAWN_VAL,
	9 * PAWN_VAL,
	9 * PAWN_VAL,
	5 * PAWN_VAL,
	5 * PAWN_VAL,
	3.25 * PAWN_VAL,
	3.25 * PAWN_VAL,
	3 * PAWN_VAL,
	3 * PAWN_VAL,
	1 * PAWN_VAL,
	1 * PAWN_VAL,
	1 * PAWN_VAL,		//ep
	-10 * PAWN_VAL,		//nopiece
};

const int32_t from_p_vals[14] = {
	-1000 * PAWN_VAL,
	-1000 * PAWN_VAL,
	9 * PAWN_VAL,
	9 * PAWN_VAL,
	5 * PAWN_VAL,
	5 * PAWN_VAL,
	3.25 * PAWN_VAL,
	3.25 * PAWN_VAL,
	3 * PAWN_VAL,
	3 * PAWN_VAL,
	1 * PAWN_VAL,
	1 * PAWN_VAL,
	1 * PAWN_VAL,		//ep
	-10 * PAWN_VAL,		//nopiece
};

#define MATCUTOFF		((int32_t)(1.50 * PAWN_VAL))
#define ASPIRATION_WINDOW	((int32_t)(0.25 * PAWN_VAL))

/* Strategic Parameters */

// this is always a positive quantity
#define CONTEMPT		((int32_t)(0))
#define COUNTVAL 0.01
#define CASTLING_BONUS (PAWN_VAL * 0.25)

const int32_t piece_vals[14] = {
1000 * PAWN_VAL,
1000 * PAWN_VAL,
9 * PAWN_VAL,
9 * PAWN_VAL,
5 * PAWN_VAL,
5 * PAWN_VAL,
3.25 * PAWN_VAL,
3.25 * PAWN_VAL,
3 * PAWN_VAL,
3 * PAWN_VAL,
1 * PAWN_VAL,
1 * PAWN_VAL,
1 * PAWN_VAL, //ep
0 * PAWN_VAL, //nopiece
};

const uint32_t control_params[64] = {
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1500,
1500,
1500,
1500,
1500,
1500,
1000,
1000,
1500,
2000,
2000,
2000,
2000,
1500,
1000,
1000,
1500,
2000,
3000,
3000,
2000,
1500,
1000,
1000,
1500,
2000,
3000,
3000,
2000,
1500,
1000,
1000,
1500,
2000,
2000,
2000,
2000,
1500,
1000,
1000,
1500,
1500,
1500,
1500,
1500,
1500,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
};

// penalties per rank for having doubled pawns
const uint32_t doubled_penalty[8] = {
-5000,
-10000,
-15000,
-20000,
-20000,
-15000,
-10000,
-5000,
};

const uint32_t WPassed_points[8] = {
0,
20000,
25000,
35000,
50000,
65000,
110000,
0,
};

const uint32_t BPassed_points[8] = {
0,
110000,
65000,
50000,
35000,
25000,
20000,
0,
};

#endif
