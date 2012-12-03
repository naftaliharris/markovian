// test_search.c
// tools for testing search and ai.c

#define __USE_STRING_INLINES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>

#include "../src/features.h"
#include "../src/bitscan.c"
#include "../src/position.h"
#include "../src/move.h"
#include "../src/move.c"
#include "../src/hash/hash.c"
#include "../src/position.c"
#include "../src/addmove.c"
#include "../src/allmoves.c"
#include "../src/makemove.c"
#include "../src/frontend.c"
#include "../src/fenio.c"
#include "../src/evaluate.c"
#include "../src/timer.c"
#include "../src/ai.c"

// Takes in a position and the ply, makes the best move, and prints the
// fen to stdout
void bestmove_fen(struct position *pos, unsigned char ply)
{
    // setup
	tt = new_trans_tables();

    // do the work
    struct move *mv = find_bestmove(pos, ply, 0);
    make_move(pos, mv);
    char *fen = pos2fen(pos);
    fprintf(stdout, "%s", fen);
    fflush(stdout);

    // cleanup
    free(tt);
}

// reads fens in from stdin and outputs the resulting fens using ply 6
int main(int argc, char **argv)
{
    struct position pos;
    char *fen;
    size_t n;

    int fen_count;
    for(fen_count = 0; ; fen_count++)
    {
        fen = NULL;
        if (getline(&fen, &n, stdin) == -1) {
            // EOL
            break;
        }
        fen2pos(&pos, fen);
        free(fen);

        bestmove_fen(&pos, 6);
    }

    return(0);
}
