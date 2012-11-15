// history.c
// basic functions for dealing with the game history struct

/* Note: this doesn't implement precisely the same repitition rules as FIDE does. FIDE considers two positions equivalent if
	1) The same side is on move
	2) Each square has the same kind of piece (ie, switches are permissible)
	3) Both sides have the same set of possible moves
The hash struct implements 1 and 2 the same way, but it records en_passant and castling permissions even if those permissions are not exercisable, unlike FIDE, which only cares if the option is exercisable. In other words, our criteria are somewhat more strict than FIDE's, which is a potential problem.
*/

#ifndef _HISTORY_C
#define _HISTORY_C

#include "history.h"

struct history *init_history(void)
{
	struct history *hist = calloc(1, sizeof(struct history));
	return(hist);
}


// checks if the position indexed by move_num is a repetition of a previous position in the history
// Note: There is no need to clear the history struct when unmoving, because the hashes will be just be overwritten later and is_rep does not use any information past move_num
int is_rep(uint16_t, uint16_t);
inline int is_rep(uint16_t limp1, uint16_t move_num)
{
	// repetitions are only possible in positions 4, 6, 8, 10,... moves ago
	for(int i = move_num - 4; i >= limp1; i-=2){
		if(hist->keys[i] == hist->keys[move_num]){
			return(1);
		}
	}
	return(0);
}

#endif
