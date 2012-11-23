// hash.c
// functions for computing hashes

#ifndef _HASH_C
#define _HASH_C

#include <string.h>

#include "hash.h"
#include "../position.h"
#include "../move.h"
#include "../move.c"

void print_hash(struct hashed *h)
{
	fprintf(stderr, "hash\t\t\tscore\tflags\tmovenum\tply\tbestmove\n");
	fprintf(stderr, "%llu\t%d\t%d\t%d\t%d\t", h->hash, h->score, h->flags,
		h->move_num, h->ply);
	print_move(&h->bestmove);
	fprintf(stderr, "\n");
}

int cache_writes;

// compute a hash from scratch (as opposed to move-unmove style)
// presumption is that this will not be used often, so no effort has been made to optimize it.
// for example, can exploit fact that both kings are always on the board, that never more than 10 rooks, 8 pawns, etc..
uint64_t make_hash(struct position *pos)
{
	uint64_t result = ToMoveHash[pos->tomove];
	result ^= CastlingHash[pos->perm_castling];
	result ^= EPHash[pos->ep];

	uint64_t locs;
	unsigned char index_from;
	for (int i = 0; i < 12; i++) {
		locs = pos->pieces[i];
		while ((index_from = ffsll(locs)) != 0) {
			index_from--;
			result ^= Hash[i][index_from];
			locs &= notlinboard[index_from];
		}
	}
	return (result);
}

struct hashed *new_trans_tables(void);
inline struct hashed *new_trans_tables(void)
{
	struct hashed *tt =
	    calloc(1 << TRANS_LOOKUP_BITS, sizeof(struct hashed));
	return (tt);
}

// reset_trans should only be called for testing purposes, as it erases the entire hash table!
void reset_trans(struct hashed *);
inline void reset_trans(struct hashed *tt)
{
	memset(tt, 0, ((1 << TRANS_LOOKUP_BITS) * sizeof(struct hashed)));
}

struct hashed *lookup_trans(uint64_t);
inline struct hashed *lookup_trans(uint64_t hash)
{
	return (&tt[hash >> (64 - TRANS_LOOKUP_BITS)]);
}

// fills in hash. (Does NOT check whether data there is old or not)
void write_hash(struct position *, struct hashed *, int32_t, unsigned char,
		unsigned char, struct move *);
inline void write_hash(struct position *pos, struct hashed *h, int32_t score,
		       unsigned char flags, unsigned char ply, struct move *mv)
{
#ifdef _CACHE_HITS
	cache_writes++;
#endif

	h->hash = pos->hash;
	h->score = score;
	memcpy(&h->bestmove, mv, sizeof(struct move));
	h->flags = flags;
	h->move_num = pos->move_num;
	h->ply = ply;
}

// fills in hash, except for move. (Does NOT check whether data there is old or not)
void write_hash_eval(struct position *, struct hashed *, int32_t, unsigned char,
		     unsigned char);
inline void write_hash_eval(struct position *pos, struct hashed *h,
			    int32_t score, unsigned char flags,
			    unsigned char ply)
{
#ifdef _CACHE_HITS
	cache_writes++;
#endif

	h->hash = pos->hash;
	h->score = score;
	memset(&h->bestmove, 0, sizeof(struct move));
	h->flags = flags;
	h->move_num = pos->move_num;
	h->ply = ply;
}

/*
 * 3 cases for hash lookups in score. (undetected collisions assumed never to occur):
 * 1)	slot is empty or with old or irrelevant info:
 * 		compute, fill.
 * 2)	slot matches with relevant info:
 * 		use.
 * 3)	slot does not match, but has relevant info:
 * 		compute, don't fill
 */

#endif
