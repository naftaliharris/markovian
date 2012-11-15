// move.c
// foundational sorts of functions on moves and move arrays

#ifndef _MOVE_C
#define _MOVE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minmax.h"
#include "position.h"
#include "move.h"

#include "hash/hash.h"

// just prints the promotion piece
char print_special(struct move *mv)
{
	switch(mv->special & PROMOMASK)
	{
		case QUEEN:
			return('q');
		case ROOK:
			return('r');
		case KNIGHT:
			return('n');
		case BISHOP:
			return('b');
		default:
			return(' '); 
	}
}

void print_move(struct move *mv)
{
	#ifndef _XBOARD
	fprintf(stdout,"%c%c %c%c %c| ",mv->from % 8 + 'a', mv->from/8 + '1', mv->to % 8 + 'a', mv->to/8 + '1', print_special(mv));
	#endif
	
	#ifdef _XBOARD
	fprintf(stdout,"%c%c%c%c%c",mv->from % 8 + 'a', mv->from/8 + '1', mv->to % 8 + 'a', mv->to/8 + '1', print_special(mv));
	#endif
}

void print_move_array(struct move_array *m)
{
	fprintf(stdout,"lsize = %d\n",m->lsize);
	fprintf(stdout,"mv     |  score      to_p\n");
	for(int i = 0; i < m->lsize; i++){
		print_move(&m->list[i].mv);
		fprintf(stdout,"%d %d\n",m->list[i].score, m->list[i].mv.to_p);
	}
}

struct move_array *new_move_array(void);
inline struct move_array *new_move_array(void)
{
	struct move_array *m = malloc(sizeof(struct move_array));
	m->list = calloc(MAXMOVES, sizeof(struct meta_move));
	m->lsize = 0;
	m->psize = MAXMOVES;
	return(m);
}

void free_move_array(struct move_array *);
inline void free_move_array(struct move_array *m)
{
	free(m->list);
	free(m);
}

void free_meta_move(struct meta_move *mm)
{
	if(mm->ma == NULL){
		return;
	}
	else {
		for(int i = 0; i < mm->ma->lsize; i++){
			free_meta_move(&mm->ma->list[i]);
		}
		free_move_array(mm->ma);
		return;
	}
}

/*
// free linked list of best moves
void free_best_moves(struct move_array *);
inline void free_best_moves(struct move_array *m)
{
	struct move_info *mi_walker1;
	struct move_info *mi_walker2;
	for(int i = 0; i < m->lsize; i++){
		mi_walker1 = m->mi_list[i];
		while(mi_walker1 != NULL){
			mi_walker2 = mi_walker1->best;
			free(mi_walker1);
			mi_walker1 = mi_walker2;
		}
	}
}

void free_moveinfo(struct move_info *);
inline void free_moveinfo(struct move_info *mi)
{
	struct move_info *mi_walker1;
	struct move_info *mi_walker2;
	mi_walker1 = mi;
	while(mi_walker1 != NULL){
		mi_walker2 = mi_walker1->best;
		free(mi_walker1);
		mi_walker1 = mi_walker2;
	}
}

// returns a pointer to a copy of to_be_copied
struct move_info *copy_move_info_once(struct move_info *);
inline struct move_info *copy_move_info_once(struct move_info *to_be_copied)
{
	struct move_info *mi = malloc(sizeof(struct move_info));	
	memcpy(mi,to_be_copied,sizeof(struct move_info));
	return(mi);
}

void copy_move_info(struct move_info *);
inline void copy_move_info(struct move_info *mi)
{
	// copy the linked list of best moves
	struct move_info *mi_walker1 = mi->best;
	while(mi_walker1->best != NULL){
		mi_walker1->best = copy_move_info_once(mi_walker1->best);
		mi_walker1 = mi_walker1->best;
	}
}
*/

#define USE_32_BIT_MULTIPLICATIONS
// transforms signature 'b' with magic 'magic' of length 'bits'
// typically, b = rook_bitboard & mask, or analogous
int magictransform(uint64_t, uint64_t, int);
inline int magictransform(uint64_t b, uint64_t magic, int bits) {
#ifdef USE_32_BIT_MULTIPLICATIONS
		return
			(unsigned)((int)b*(int)magic ^ (int)(b>>32)*(int)(magic>>32)) >> (32-bits);
#else
		return (int)((b * magic) >> (64 - bits));
#endif
}

unsigned char find_piece(struct position *, unsigned char);
inline unsigned char find_piece(struct position *pos, unsigned char loc)
{
	// NOTE: does NOT look in ep_squares[pos->towait][pos->ep], since this function MUST be called only by non-pawns attacks
	// can obviously be optimized
	for(int i = 0; i < 12; i++){
		if(linboard[loc] & pos->pieces[i]){
			return(i);
		}
	}
	return(nopiece_n); //didn't find any piece there
}

unsigned char find_piece_ep(struct position *, unsigned char);
inline unsigned char find_piece_ep(struct position *pos, unsigned char loc)
{
	// NOTE: DOES look in ep_squares[pos->towait][pos->ep], since this function MUST be called only by pawns
	// can obviously be optimized
	for(int i = 0; i < 12; i++){
		if(linboard[loc] & pos->pieces[i]){
			return(i);
		}
	}
	if(linboard[loc] & ep_squares[pos->towait][pos->ep]){
		return(eppiece_n);
	}
	return(nopiece_n); //didn't find any piece or ep trace there
}

#endif
