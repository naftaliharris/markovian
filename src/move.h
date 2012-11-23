// move.h
// definitions for moves

/* HOW IT ALL WILL WORK:
 * struct move is a self-contained move. Given a struct move, you can easily make the move.
 * struct move_array is a self-contained list of moves. It contains only a list of moves and info about how many there are 
 * struct move_info is a struct returned by score and quiet. It contains all data generated from that branch in the tree:
 * 	-- The score of the move-branch
 * 	-- The move generation after the move
 * 	-- The returned move_info's from the move's moves.
 *
 * Facts:
 * There must be a way to sort the moves together with their move_info's 
 * 	=> Therefore we need to group together moves and move_info's in a central struct, and have a list of those.
 *		=> Having only a list of moves, without information about them, is useless. So must change or delete struct move_array
 */

#ifndef _MOVE
#define _MOVE
struct move
{
	unsigned char	from;
	unsigned char	to;
	
	unsigned char	from_p;
	unsigned char	to_p; //piece from and to, 0 through 11. (For easy access).

	//the below are not set in stone. Consider special flags for capture, etc
	unsigned char	castling;
	//castling defined as in position.h
	unsigned char	special;
	//defined as in position.h
};

#define MAXMOVES 128 //maximum moves in a position

struct move_array
{
	unsigned int		lsize; //logical size
	unsigned int		psize; //physical size
	struct meta_move	*list;
};

// the below can be sorted
// it is passed as part of argument to void score and void quiet, which modify score and move_array.
struct meta_move
{
	struct move		mv; 	// the move considered (untouched by score and quiet)
	int32_t			score;  // the score of the branch from that move (changed by score and quiet)
	struct move_array	*ma;	// the move array from that move (either used or created by score and quiet)
	#ifdef _NODE_COUNT
	int			nodes;
	#endif
	unsigned char		counted; // whether the move has been made yet or not
	#define COUNTED 1
	#define UNCOUNTED 0
};

/* pseudo code for score
 * void score(pos, ply, ..., struct meta_move mm)
 * {
 * 	if(pos->score is checkmate){
 * 		mm->score = pos->score;
 * 		mm->ma = NULL;
 * 		return;
 * 	}
 * 	else {
 * 		if(ply == 0){
 * 			quiet(pos, mm->ma);
 * 			return;
 * 		}
 * 		if(mm->ma == NULL){
 * 			mm->ma = allmoves(pos);
 * 			qsort by captures;
 * 		}
 * 		else {
 * 			qsort by scores;
 * 		}
 * 		for(int i = 0; i < mm->ma->lsize; i++){
 * 			make move;
 * 			score(pos_tmp, ply - 1, mm->ma[i]);
 * 		}
 * 		mm->score = best score;
 * 		return;
 * 	}
 * }
 *
 * void quiet(pos, struct meta_move mm)
 * {
 * 	if(pos->score is checkmate){
 * 		mm->score = pos->score;
 * 		mm->ma = NULL;
 * 		return;
 * 	}
 * 	else {
 * 		if(mm->ma == NULL){
 * 			mm->ma = allmoves(pos);
 * 			qsort by captures;
 * 			if(no possible captures){
 * 				mm->score = evaluate(score);
 * 				return;
 * 			}
 * 		}
 * 		else {
 * 			qsort by scores;
 * 		}
 * 		for(int i = 0; i < mm->ma->lsize; i++){
 * 			make move;
 * 			quiet(pos_tmp, mm->ma[i]);
 * 		}
 * 	}
 * }
 */

#endif
