// bitscan.c
// attempt to implement fast bitscans and other bit-level operations

#include <stdint.h>

/*
#define ffsll FFSLL
int FFSLL(long long int);
inline int FFSLL(long long int x)
{
	return(__builtin_ffsll(x));
}
*/

#define ffsll(x) ( __builtin_ffsll(x) )


// counts the number of ones in a word. Optimal for sparse words
//
// key observation in the below:
// x - 1 is the same as x, but rightmost string of 0's are 1's, and rightmost 1 is a zero. For example:
// 10011000 - 1 =
// 10010111. And the two together to get:
// 10010000, and repeat
int sparse_popcount(uint64_t);
inline int sparse_popcount(uint64_t x)
{
	int count;
	for(count = 0; x; count++){
		x &= x - 1;
	}
	return(count);
}
