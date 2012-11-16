// ai.c
// artificial intelligence functions
// threaded

/* NEGAMAX FACTS:
 * 1)	Evaluation function and pos->score ALWAYS return the score relative to white, ie, higher numbers mean white is doing better
 * 2)	multiplier * evaluation(pos) and multiplier * pos->score returns the score relative to side on move, ie, higher numbers mean the side to move is doing better
 * 3)	ALWAYS return and cache scores relative to the side on move, so that higher children scores are worse parent scores, and higher cached scores are always better for the side to move
 * 4)	So when looking at work results from children nodes,  mm_select->score will always be relative to side to wait, ie, higher numbers mean the side to wait is doing better
 * 5)	So when looking at work results from children nodes, -mm_select->score will always be relative to side to move, ie, higher numbers mean the side to move is doing better
 *
 *  alpha and beta:
 *  lower is the best score so far for the side to MOVE,  higher is the best score so far for the side to WAIT, (relative to the side to MOVE), so
 * -lower is the best score so far for the side to WAIT, -higher is the best score so far for the side to MOVE, (relative to the side to WAIT).
 *
 *  move sorting by score:
 *  the side to move always wants the highest scores relative to itself, or the lowest scores relative to tomove.  Since the results in mm->ma->list are relative to towait, it sorts them by score in ascending order.
 *
 *  bestmm->score -= TOLERANCE, mm->score = our_low + TOLERANCE:
 *  upon an alpha beta cutoff, need to signal to tomove that this line may be even worse. (See TODO.txt 45). The signal MUST BE AT LEAST AS BIG AS TOLERANCE! (See TODO.txt 45).
 *
 *  contempt: Upon a draw, the position is always evaluated as multiplier * contempt. for the side for which the computer is making the move, multiplier * contempt must be negative. For the side for which the computer is not making the move, multiplier * contempt must be positive. CONTEMPT is always positive, therefore in computer_move we set contempt = -multiplier * CONTEMPT.
    if the computer plays white: contempt = -CONTEMPT, white returns -CONTEMPT, black returns  CONTEMPT.
    if the computer plays black: contempt =  CONTEMPT, white returns  CONTEMPT, black returns -CONTEMPT.
 */

//#include <pthread.h>
#include "dequietmoves.c"
#include "history.c"
#include "hash/hash.c"
#include "evaluate.c"
#include "allmoves.c"
#include "makemove.c"

#include <time.h>

#ifdef _CACHE_HITS
#include "cache_hits.c"
#endif

#define MINUSINFINITY		((int32_t)(-10000 * PAWN_VAL))
#define PLUSINFINITY		((int32_t)(10000 * PAWN_VAL))
//#define TOLERANCE             ((int32_t)(PAWN_VAL / 1000))
#define TOLERANCE		0
#define MATCUTOFF		((int32_t)(1.50 * PAWN_VAL))	//be very VERY careful with this parameter when you change your score function!!!
#define ASPIRATION_WINDOW	((int32_t)(0.25 * PAWN_VAL))
#define CONTEMPT		((int32_t)(0))	// this is always a positive quantity
//#define CONTEMPT              ((int32_t)(PAWN_VAL / 2)) // this is always a positive quantity

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

// returns a heuristic "goodness" of a move, with higher numbers indicating more promising moves
// for use in select_mm
int move_goodness(struct meta_move *);
inline int move_goodness(struct meta_move *mm)
{
	/*
	   switch(mm->mv.to_p)
	   {
	   case nopiece_n:
	 */
	return (to_p_vals[mm->mv.to_p] + from_p_vals[mm->mv.from_p]);
}

// selects a move from a list of moves, (selection sort). No prior scores assumed to be known about them.
// returns NULL upon exhausting all moves, does NOT automatically mark moves as COUNTED
// This needs to be fixed: currently pawn moves to nowhere are being incentivized, (0 - 1 = -1, relative high).
struct meta_move *select_mm(struct move_array *);
inline struct meta_move *select_mm(struct move_array *ma)
{
	struct meta_move *mm = NULL;
	int new_goodness;
	int best_goodness = MINUSINFINITY;
	for (int i = 0; i < ma->lsize; i++) {
		if (ma->list[i].counted == UNCOUNTED) {
			new_goodness = move_goodness(&ma->list[i]);
			if (best_goodness < new_goodness) {
				mm = &ma->list[i];
				best_goodness = new_goodness;
			}
		}
	}

	return (mm);
}

// implements negamaxed, cached, alpha-beta quiesient searches
void quiet(struct position *pos, unsigned char depth, int32_t low, int32_t high,
	   int multiplier, int32_t contempt, struct meta_move *mm)
{
	assert(high > low + TOLERANCE);

	/*
	   #ifdef _NODE_COUNT
	   mm->nodes++;
	   #endif
	 */

	//if our king has been captured, stop computation to avoid capturing back! (fails for stalemates)
	if (pos->score * multiplier < -KING_CAPTURED_SCORE) {
		//mm->score = multiplier * pos->score;
		mm->score = -(KING_VAL - depth * PAWN_VAL);
		mm->ma = NULL;
		return;
	}
	assert(pos->score > -KING_CAPTURED_SCORE
	       && pos->score < KING_CAPTURED_SCORE);

	if (high + MATCUTOFF <= multiplier * pos->score) {
		mm->score = multiplier * pos->score;	//no need to compute full evaluation if down major material because we know there is gonna be an alpha-beta cutoff.
		mm->ma = NULL;
		return;
	}
#ifdef _USE_HISTORY
	// currently, quiet only looks at captures, making checking for repititions irrelavant except for when first entering quiet. But it should move quickly anyway, and it is good form for when we later examine checks.
	if (is_rep(pos->limp1, pos->move_num) || (pos->half_move_clock >= 100)) {
		mm->score = multiplier * contempt;
		mm->ma = NULL;
		return;
	}
#endif

#ifdef _CACHE_HITS
	quiet_ops++;
#endif

	int32_t our_low = low;
	int32_t our_high = high;

#ifdef _USE_QUIET_HASH
	struct hashed *h = lookup_trans(pos->hash);
	if (h->hash == pos->hash) {
		if ((h->flags & HASH_EXACT_MASK) == HASH_EXACT) {
			mm->score = h->score;
			mm->ma = NULL;

#ifdef _CACHE_HITS
			quiet_exact_hits++;
#endif
			return;
		} else {	// can still use information even if only have lower bound
			assert(our_high > our_low + TOLERANCE);
			our_low = max(our_low, h->score);
			if (our_high <= our_low + TOLERANCE) {
				mm->score = our_low + TOLERANCE;
				//mm->score = our_low;
				mm->ma = NULL;

#ifdef _CACHE_HITS
				quiet_lb_cutoff_hits++;
#endif
				return;
			}
#ifdef _CACHE_HITS
			quiet_lb_nocutoff_hits++;
#endif
		}
	}
#endif

	// can choose not to do captures by taking current evaluation, not the capture evaluation, (hence max).
	int32_t now_score = multiplier * evaluate(pos);
	assert(our_high > our_low + TOLERANCE);
	our_low = max(now_score, our_low);
	if (our_high <= our_low + TOLERANCE) {
		mm->score = our_low + TOLERANCE;
		//mm->score = our_low;
		mm->ma = NULL;

#ifdef _USE_QUIET_HASH
		if ((h->hash == 0) || (h->flags >= (HASH_LB | HASH_QUIET))) {
			write_hash_eval(pos, h, mm->score,
					(HASH_LB | HASH_QUIET), HASH_QUIET_PLY);
#ifdef _CACHE_HITS
			quiet_writes++;
#endif
		}
#endif

		return;
	}

	mm->ma = dequietmoves(pos);

	// if no more captures, return evalation
	if (mm->ma->lsize == 0) {
		mm->score = now_score;
		free_move_array(mm->ma);
		mm->ma = NULL;

#ifdef _USE_QUIET_HASH
		// possibly cache this result. Almost always should cache, exception being if analyzed by score
		if ((h->hash == 0) || (h->flags >= (HASH_EXACT | HASH_QUIET))) {
			write_hash_eval(pos, h, mm->score,
					(HASH_EXACT | HASH_QUIET),
					HASH_QUIET_PLY);
#ifdef _CACHE_HITS
			quiet_writes++;
#endif
		}
#endif

		return;
	}

	struct position *pos_tmp = malloc(sizeof(struct position));
	struct meta_move *mm_select = NULL;
	struct meta_move *bestmm = NULL;
	unsigned char flags = (HASH_EXACT | HASH_QUIET);
	int lsize = mm->ma->lsize;

#ifdef _USE_QUIET_HASH
	// try the hash move first: doesn't always exist, even for LB, because of cutoffs from analyzing own position
	if ((h->hash == pos->hash) && (h->bestmove.to_p != h->bestmove.from_p)) {
		assert(consistency(pos));
		assert(our_high > our_low + TOLERANCE);
		memcpy(pos_tmp, pos, sizeof(struct position));

		//find where the hash move is in the list
		for (int i = 0; i < mm->ma->lsize; i++) {
			if (memcmp
			    (&mm->ma->list[i].mv, &h->bestmove,
			     sizeof(struct move)) == 0) {
				mm_select = &mm->ma->list[i];
				break;
			}
		}
		if (mm_select != NULL) {
#ifdef _CACHE_HITS
			quiet_move_hits++;
#endif

			mm_select->counted = COUNTED;
			lsize--;

			make_move(pos_tmp, &h->bestmove);
			quiet(pos_tmp, depth + 1, -our_high, -our_low,
			      -multiplier, contempt, mm_select);

			// check if best move
			bestmm = mm_select;
			if (-bestmm->score > our_low) {
				our_low = -bestmm->score;
				if (our_high <= our_low + TOLERANCE) {
					flags = (HASH_LB | HASH_QUIET);
					bestmm->score -= TOLERANCE;
					goto label_quiet_done;
				}
			}
		} else {
			assert(h->flags == (HASH_LB | HASH_SCORE));
			// what's happening is that earlier in score, we found that the best move was not a capture, and so now we can't find this best move in our list of captures.
			// we should probably use this information more
		}
	}
#endif

	// next try all moves except last one. (When try last one, can get exact score)
	for (int i = 0; i < lsize - 1; i++) {
		mm_select = select_mm(mm->ma);
		assert(mm_select != NULL);

		mm_select->counted = COUNTED;

		assert(consistency(pos));
		assert(our_high > our_low + TOLERANCE);
		memcpy(pos_tmp, pos, sizeof(struct position));

		make_move(pos_tmp, &mm_select->mv);
		quiet(pos_tmp, depth + 1, -our_high, -our_low, -multiplier,
		      contempt, mm_select);

		/*
		   #ifdef _NODE_COUNT
		   mm->nodes += mm->ma->list[i].nodes;
		   #endif
		 */

		// check if best move
		if ((bestmm == NULL) || (-mm_select->score > -bestmm->score)) {
			bestmm = mm_select;
			if (-bestmm->score > our_low) {
				our_low = -bestmm->score;
				if (our_high <= our_low + TOLERANCE) {
					flags = (HASH_LB | HASH_QUIET);
					bestmm->score -= TOLERANCE;
					goto label_quiet_done;
				}
			}
		}
	}

	// finally, try the last move, automatically going to be HASH_EXACT
	if (lsize > 0) {
		mm_select = select_mm(mm->ma);
		assert(mm_select != NULL);

		mm_select->counted = COUNTED;

		assert(consistency(pos));
		assert(our_high > our_low + TOLERANCE);
		memcpy(pos_tmp, pos, sizeof(struct position));

		make_move(pos_tmp, &mm_select->mv);
		quiet(pos_tmp, depth + 1, -our_high, -our_low, -multiplier,
		      contempt, mm_select);

		// check if best move, for possible later use by score, say. NO CUTOFFS here.
		if ((bestmm == NULL) || (-mm_select->score > -bestmm->score)) {
			bestmm = mm_select;
			if (-bestmm->score > our_low) {
				our_low = -bestmm->score;
			}
		}
		/*
		   if(-mm_select->score > our_low){
		   our_low = -mm_select->score;
		   bestmm = mm_select;
		   }
		 */
	}

 label_quiet_done:
	free(pos_tmp);

	// add the best score
	//mm->score = our_low;
	mm->score = max(now_score, -bestmm->score);

#ifdef _USE_QUIET_HASH
	// possibly cache this result. Almost always should cache, exception being if analyzed by score or by quiet exact and we're lower bound.
	if ((h->hash == 0) || (h->flags >= flags)) {
		assert((h->hash == 0)
		       || ((h->flags & HASH_SCORE_MASK) == HASH_QUIET));
		switch ((int)bestmm) {
		case (int)NULL:	//if can do the best by not moving, must have looked all all possible captures, so exact
			assert((flags & HASH_EXACT_MASK) == HASH_EXACT);
			write_hash_eval(pos, h, mm->score, flags,
					HASH_QUIET_PLY);

#ifdef _CACHE_HITS
			quiet_writes++;
#endif
			break;
		default:
			assert(lookup_trans(pos->hash) == h);
#ifdef _DEBUG
			char found = 0;
			for (int i = 0; i < mm->ma->lsize; i++) {
				if (memcmp
				    (&mm->ma->list[i].mv, &bestmm->mv,
				     sizeof(struct move)) == 0) {
					found = 1;
					break;
				}
			}
			assert(found);
#endif
			write_hash(pos, h, mm->score, flags, HASH_QUIET_PLY,
				   &bestmm->mv);

#ifdef _CACHE_HITS
			quiet_writes++;
#endif
			break;
		}
	}
#endif

	free_move_array(mm->ma);
	mm->ma = NULL;

	return;
}

// negamaxed alpha-beta pruning with hashing
void score(struct position *pos, unsigned char ply, unsigned char depth,
	   int32_t low, int32_t high, int multiplier, int32_t contempt,
	   struct meta_move *mm)
{
	assert(high > low + TOLERANCE);

#ifdef _NODE_COUNT
	mm->nodes++;
#endif

	if (ply == 0) {
#ifdef _USE_QUIET
		quiet(pos, depth, low, high, multiplier, contempt, mm);
#else
		if (high + MATCUTOFF <= multiplier * pos->score) {
			mm->score = multiplier * pos->score;	//no need to compute full evaluation if down major material
			mm->ma = NULL;
		} else {
			mm->score = multiplier * evaluate(pos);
		}
#endif
		return;
	} else {
		//if king has been captured, stop computation to avoid capturing back! (fails for stalemates)
		//this check only occurs if ply > 0 because it occurs at the beginning of quiet, and no sense repeating it.
		if (pos->score * multiplier < -KING_CAPTURED_SCORE) {
			//mm->score = multiplier * pos->score;
			mm->score = -(KING_VAL - depth * PAWN_VAL);
			mm->ma = NULL;
			return;
		}
		assert(pos->score > -KING_CAPTURED_SCORE
		       && pos->score < KING_CAPTURED_SCORE);

#ifdef _USE_HISTORY
		//this check only occurs if ply > 0 because it occurs at the beginning of quiet, and no sense repeating it.
		if (is_rep(pos->limp1, pos->move_num)
		    || (pos->half_move_clock >= 100)) {
			mm->score = multiplier * contempt;
			mm->ma = NULL;
			return;
		}
#endif

#ifdef _CACHE_HITS
		score_ops++;
#endif
		int32_t our_low = low;
		int32_t our_high = high;

#ifdef _USE_SCORE_HASH
		struct hashed *h = lookup_trans(pos->hash);
		if ((h->hash == pos->hash) && h->ply >= ply) {	//match with relevant info
			if (h->flags == (HASH_EXACT | HASH_SCORE)) {
				mm->score = h->score;
#ifdef _CACHE_HITS
				score_exact_hits++;
#endif
				return;
			} else {	// can still use information even if only have lower bound
				our_low = max(our_low, h->score);
				if (our_high <= our_low + TOLERANCE) {
					mm->score = our_low + TOLERANCE;
					//mm->score = our_low;
#ifdef _CACHE_HITS
					score_lb_cutoff_hits++;
#endif
					return;
				}
#ifdef _CACHE_HITS
				score_lb_nocutoff_hits++;
#endif
			}
		}
#endif

		struct meta_move *bestmm = NULL;
		unsigned char flags = (HASH_EXACT | HASH_SCORE);
		struct position *pos_tmp = malloc(sizeof(struct position));
		struct meta_move *mm_select = NULL;

		mm->ma = allmoves(pos);

		int lsize = mm->ma->lsize;

#ifdef _USE_SCORE_HASH
		// try the hash move first: doesn't always exist, even for LB, because of cutoffs from analyzing own position in quiet
		if ((h->hash == pos->hash)
		    && (h->bestmove.to_p != h->bestmove.from_p)) {
#ifdef _CACHE_HITS
			score_move_hits++;
#endif
			assert(consistency(pos));
			assert(our_high > our_low + TOLERANCE);
			memcpy(pos_tmp, pos, sizeof(struct position));

			//find where the hash move is in the list
			for (int i = 0; i < mm->ma->lsize; i++) {
				if (memcmp
				    (&mm->ma->list[i].mv, &h->bestmove,
				     sizeof(struct move)) == 0) {
					mm_select = &mm->ma->list[i];
					break;
				}
			}
			assert(mm_select != NULL);	// we have a list of all moves, so no excuse for not finding the hashed move in our list

			mm_select->counted = COUNTED;
			lsize--;

			make_move(pos_tmp, &h->bestmove);
			score(pos_tmp, ply - 1, depth + 1, -our_high, -our_low,
			      -multiplier, contempt, mm_select);

#ifdef _NODE_COUNT
			mm->nodes += mm_select->nodes;
#endif

			// check if best move
			if ((bestmm == NULL)
			    || (-mm_select->score > -bestmm->score)) {
				bestmm = mm_select;
				if (-bestmm->score > our_low) {
					our_low = -bestmm->score;
					if (our_high <= our_low + TOLERANCE) {
						flags = (HASH_LB | HASH_SCORE);
						bestmm->score -= TOLERANCE;
						goto label_score_done;
					}
				}
			}
		}
#endif

		// next try all moves except last one. (When try last one, can get exact score)
		for (int i = 0; i < lsize - 1; i++) {
			mm_select = select_mm(mm->ma);
			assert(mm_select != NULL);

			mm_select->counted = COUNTED;

			assert(consistency(pos));
			assert(our_high > our_low + TOLERANCE);
			memcpy(pos_tmp, pos, sizeof(struct position));

			make_move(pos_tmp, &mm_select->mv);
			score(pos_tmp, ply - 1, depth + 1, -our_high, -our_low,
			      -multiplier, contempt, mm_select);

#ifdef _NODE_COUNT
			mm->nodes += mm_select->nodes;
#endif

			// check if best move
			if ((bestmm == NULL)
			    || (-mm_select->score > -bestmm->score)) {
				bestmm = mm_select;
				if (-bestmm->score > our_low) {
					our_low = -bestmm->score;
					if (our_high <= our_low + TOLERANCE) {
						flags = (HASH_LB | HASH_SCORE);
						bestmm->score -= TOLERANCE;
						goto label_score_done;
					}
				}
			}
		}

		// finally, try the last move, automatically going to be HASH_EXACT
		if (lsize > 0) {
			mm_select = select_mm(mm->ma);
			assert(mm_select != NULL);

			mm_select->counted = COUNTED;

			assert(consistency(pos));
			assert(our_high > our_low + TOLERANCE);
			memcpy(pos_tmp, pos, sizeof(struct position));

			make_move(pos_tmp, &mm_select->mv);
			score(pos_tmp, ply - 1, depth + 1, -our_high, -our_low,
			      -multiplier, contempt, mm_select);

#ifdef _NODE_COUNT
			mm->nodes += mm_select->nodes;
#endif

			// check if best move, for possible later use by score, say. NO CUTOFFS here.
			if ((bestmm == NULL)
			    || (-mm_select->score > -bestmm->score)) {
				bestmm = mm_select;
				if (-bestmm->score > our_low) {
					our_low = -bestmm->score;
				}
			}
		}

 label_score_done:
		free(pos_tmp);

		// add the best score
		//mm->score = our_low;
		mm->score = -bestmm->score;

#ifdef _USE_SCORE_HASH
		// possibly cache this result. Almost always should cache, exception being if no match, and from an earlier position.
		if ((h->hash == 0) || (h->flags > flags)
		    || ((int)h->move_num + h->ply <=
			(int)pos->move_num + ply)) {
			write_hash(pos, h, mm->score, flags, ply, &bestmm->mv);
#ifdef _CACHE_HITS
			score_writes++;
#endif
		}
#endif

		free_move_array(mm->ma);
		mm->ma = NULL;

		return;
	}
}

void computer_move(struct position *pos, unsigned char ply)
{
	int multiplier = pos->tomove * -2 + 1;
	int contempt = -multiplier * CONTEMPT;

	struct meta_move *mm = calloc(1, sizeof(struct meta_move));
#ifdef _NODE_COUNT
	mm->nodes = 0;
#endif

#ifdef _CACHE_HITS
	zero_cache_counts();
#endif

	// iterative deepening
	struct timespec ts_i, ts_f;
	clock_gettime(CLOCK_REALTIME, &ts_i);
	clock_gettime(CLOCK_REALTIME, &ts_f);

	score(pos, 1, 0, MINUSINFINITY, PLUSINFINITY, multiplier, contempt, mm);

#ifdef _IT_DEP
	int ply_tmp = 2;
	while (minus_time(&ts_f, &ts_i) < 3000000000)	// 3 seconds
#else
	for (int ply_tmp = 2; ply_tmp <= ply; ply_tmp++)
#endif
	{
#ifdef _USE_ASPIRATION_SEARCH
		// attempt aspiration search
		int32_t old_score = multiplier * mm->score;
		score(pos, ply_tmp, 0, old_score - ASPIRATION_WINDOW,
		      old_score + ASPIRATION_WINDOW, multiplier, contempt, mm);
		if ((multiplier * mm->score > old_score + ASPIRATION_WINDOW)
		    || (multiplier * mm->score <
			old_score - ASPIRATION_WINDOW)) {
			fprintf(stderr, "Failed aspiration search\n");
			score(pos, ply_tmp, 0, MINUSINFINITY, PLUSINFINITY,
			      multiplier, contempt, mm);
		}
#else
		score(pos, ply_tmp, 0, MINUSINFINITY, PLUSINFINITY, multiplier,
		      contempt, mm);
#endif

#ifdef _IT_DEP
		clock_gettime(CLOCK_REALTIME, &ts_f);
#endif

#ifdef _XBOARD
#ifdef _IT_DEP
#ifdef _NODE_COUNT
		fprintf(stdout, "%d %d %llu %d ", ply_tmp,
			multiplier * mm->score / CENTIPAWN, minus_time(&ts_f,
								       &ts_i) /
			10000000, mm->nodes);
#else
		fprintf(stdout, "%d %d %llu 1000000 ", ply_tmp,
			multiplier * mm->score / CENTIPAWN, minus_time(&ts_f,
								       &ts_i) /
			10000000);
#endif
#endif
#endif

#ifndef _USE_SCORE_HASH
#error "Score hash must always be used for now."
#else
		struct position *pos_tmp = malloc(sizeof(struct position));
		memcpy(pos_tmp, pos, sizeof(struct position));
		struct hashed *h = lookup_trans(pos_tmp->hash);

		int moves_ahead = 0;	// to prevent infinite printing in 3-fold repetition.
		while ((h->hash == pos_tmp->hash)
		       && (h->bestmove.to != h->bestmove.from)
		       && (h->flags == (HASH_SCORE | HASH_EXACT))
		       && (moves_ahead < 20)) {
			print_move(&h->bestmove);
#ifdef _XBOARD
			fprintf(stdout, " ");
#endif

			make_move(pos_tmp, &h->bestmove);
			h = lookup_trans(pos_tmp->hash);
			moves_ahead++;
		}
		free(pos_tmp);
#endif

#ifndef _XBOARD
		fprintf(stdout, "%d\n", multiplier * mm->score);
#else
		fprintf(stdout, "\n");
#endif

#ifdef _IT_DEP
		ply_tmp++;
#endif

#ifdef _CACHE_HITS
		print_cache_hits();
#endif

	}

	// Look in the hashtable to find our best move
	struct hashed *h = lookup_trans(pos->hash);
	struct move *bestmove = &h->bestmove;

	make_move(pos, bestmove);

#ifdef _XBOARD
	fprintf(stdout, "move ");
	print_move(bestmove);
	fprintf(stdout, "\n");
#else
#ifdef _NODE_COUNT
	fprintf(stdout, "Nodes: %d\n", mm->nodes);
#endif
#endif

	free_meta_move(mm);
	free(mm);
}
