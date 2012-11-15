// history.h
// struct for recording history of positions in order to detect repetitions.

#ifndef _HISTORY_H
#define _HISTORY_H

#define EMPTY_KEY 0
#define LONGEST_GAME 1000 // longest permissible game, in half moves. This is only 7.8 kB of hashes.

struct history
{
	uint64_t keys[LONGEST_GAME]; // a list of the hashes of previous positions
};

struct history *hist; // the global pointer to the history

#endif
