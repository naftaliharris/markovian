// bitboard_test.c
// tests the constants defined in bitboard

#include "../src/bitboard.h"
#include "../src/bitscan.c"

int bitboard_test(int verbose)
{
    int failures = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(sparse_popcount(board[i][j]) != 1)
                failures++;
            if(sparse_popcount(rank[i]) != 8)
                failures++;
            if(sparse_popcount(file[j]) != 8)
                failures++;
            if((rank[i] & file[j]) != board[i][j])
                failures++;
            if(linboard[8*i + j] != board[i][j])
                failures++;
            if(notlinboard[8*i + j] != ~board[i][j])
                failures++;
        }
    }

    return(failures);
}
