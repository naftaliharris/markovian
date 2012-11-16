// addmove.c
// functions for adding moves to a struct move_array

#ifndef _ADDMOVE_C
#define _ADDMOVE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minmax.h"
#include "position.h"
#include "move.h"

// no castling or promotion here
void add_move(struct move_array *, unsigned char, unsigned char, unsigned char,
	      unsigned char);
inline void add_move(struct move_array *m, unsigned char from, unsigned char to,
		     unsigned char from_p, unsigned char to_p)
{				// no memory optimization yet; just malloc 128 moves always.
	assert(from < 64);
	assert(to < 64);
	unsigned int lsize = m->lsize;
	assert(lsize < 256);
	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	//m->list[lsize].mv.special = EMPTY;
	m->list[lsize].mv.special = 0;
	m->lsize++;
}

// for adding moves where pawns move forward by 2, (for ep)
void add_move_forward2(struct move_array *, unsigned char, unsigned char,
		       unsigned char, unsigned char);
inline void add_move_forward2(struct move_array *m, unsigned char from,
			      unsigned char to, unsigned char from_p,
			      unsigned char to_p)
{				// no memory optimization yet; just malloc 128 moves always.
	assert((from / 8 == 1) || (from / 8 == 6));
	assert((to / 8 == 3) || (to / 8 == 4));
	unsigned int lsize = m->lsize;
	assert(lsize < 256);
	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	m->list[lsize].mv.special = ADVANCE2;
	m->lsize++;
}

void add_castling_move(struct move_array *, unsigned char);
inline void add_castling_move(struct move_array *m, unsigned char type)
{				// no memory optimization yet; just malloc 128 moves always.
	unsigned int lsize = m->lsize;
	assert(lsize < 256);
	m->list[lsize].mv.to_p = nopiece_n;
	m->list[lsize].mv.castling = type;
	m->list[lsize].mv.special = EMPTY;
	switch (type) {
	case WKING:
		m->list[lsize].mv.from = 4;
		m->list[lsize].mv.to = 6;
		m->list[lsize].mv.from_p = wking_n;
		break;
	case BKING:
		m->list[lsize].mv.from = 60;
		m->list[lsize].mv.to = 62;
		m->list[lsize].mv.from_p = bking_n;
		break;
	case WQUEEN:
		m->list[lsize].mv.from = 4;
		m->list[lsize].mv.to = 2;
		m->list[lsize].mv.from_p = wking_n;
		break;
	case BQUEEN:
		m->list[lsize].mv.from = 60;
		m->list[lsize].mv.to = 58;
		m->list[lsize].mv.from_p = bking_n;
		break;
	}
	m->lsize++;
}

//not inlined because rarely called
void add_promotion_move(struct move_array *m, unsigned char from,
			unsigned char to, unsigned char from_p,
			unsigned char to_p)
{
	assert(from < 64);
	assert(to < 64);
	unsigned char color = from_p & COLOR;
	unsigned int lsize = m->lsize;
	assert(lsize < 252);

	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	m->list[lsize].mv.special = QUEEN | color;
	m->lsize++;
	lsize = m->lsize;

	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	m->list[lsize].mv.special = KNIGHT | color;
	m->lsize++;
	lsize = m->lsize;

	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	m->list[lsize].mv.special = ROOK | color;
	m->lsize++;
	lsize = m->lsize;

	m->list[lsize].mv.from = from;
	m->list[lsize].mv.to = to;
	m->list[lsize].mv.from_p = from_p;
	m->list[lsize].mv.to_p = to_p;
	m->list[lsize].mv.castling = NONE;
	m->list[lsize].mv.special = BISHOP | color;
	m->lsize++;
}

#endif
