// evaluate.c
// evaluates quiet positions

#ifndef _EVALUATE_C
#define _EVALUATE_C

#include <math.h>

#include "evaluation/control_eval.c"
#include "evaluation/pawn_eval.c"
#include "countmoves.c"

// Will return evaluation relative to white; higher numbers are better for white
int32_t evaluate(struct position *pos)
{
	uint32_t result = 0;
	
	#ifdef _USE_CONTROL_EVAL
	result += control_eval(pos);
	#endif

	#ifdef _USE_PAWN_EVAL
	result += pawn_eval(pos);
	#endif

	#ifdef _USE_COUNT_EVAL
	char tomove = pos->tomove;
	char towait = pos->towait;

	pos->tomove = WHITE;
	pos->towait = BLACK;
	result += countmoves(pos) * PAWN_VAL / 100;

	pos->tomove = BLACK;
	pos->towait = WHITE;
	result -= countmoves(pos) * PAWN_VAL / 100;
	
	pos->tomove = tomove;
	pos->towait = towait;
	#endif

	result += pos->score;

	// NOTE: Be careful about scaling!!!
	//return((int32_t)lroundf((pos->score + PAWN_VAL * result)));
	return(result);
}

#endif
