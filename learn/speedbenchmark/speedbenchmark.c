// speedbenchmark.c
// a routine for testing Markovian on speedbenchmark.fens

#define __USE_STRING_INLINES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "../../src/features.h"
#include "../../src/bitscan.c"
#include "../../src/position.h"
#include "../../src/move.h"
#include "../../src/move.c"
#include "../../src/hash/hash.c"
#include "../../src/position.c"
#include "../../src/addmove.c"
#include "../../src/allmoves.c"
#include "../../src/makemove.c"
#include "../../src/frontend.c"
#include "../../src/fenio.c"
#include "../../src/evaluate.c"
#include "../../src/timer.c"
#include "../../src/ai.c"

// times a position up to ply
// it would be better if we had all the moves, instead of just the position
// so that we could construct the history and be more realistic
unsigned long long timeposition(struct position *pos, unsigned char ply)
{
    // setup
	tt = new_trans_tables();
	struct timespec cput_old, cput_new;

    // time it
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cput_old);
    find_bestmove(pos, ply, 0);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cput_new);

    // cleanup
    free(tt);
    
    return((unsigned long long) minus_time(&cput_new, &cput_old));
}

void usage(char *name)
{
    fprintf(stderr, "Usage: %s [--new-plies seconds]\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    unsigned long long over_time = 0;
    if(argc == 3)
    {
        if(strcmp(argv[1], "--new-plies") != 0)
        {
            usage(argv[0]);
        }
        double time_s = atof(argv[2]);
        over_time = (unsigned long long) (BILLION * time_s);
    }
    else if(argc != 1)
    {
        usage(argv[0]);
    }

    struct position pos;
    FILE *infile = fopen("speedbenchmark.fens", "r");
    FILE *plyfile = (over_time == 0) ? fopen("speedbenchmark.plies", "r") : fopen("speedbenchmark.plies", "w");
    char *fen;
    size_t n;
    unsigned long long total_time = 0;

    for(int i = 0; i < 500; i++)
    {
        fen = NULL;
        while (getline(&fen, &n, infile) == -1) {
            fprintf(stderr, "Failed getline\n");
        }
        fen2pos(&pos, fen);
        free(fen);

        unsigned long long timed;
        if(over_time != 0)
        {
            // XXX: Very inefficient
            unsigned char ply = 2;
            while((timed = timeposition(&pos, ply)) < over_time)
            {
                ply++;
            }
            fprintf(plyfile, "%d\n", ply);
            fflush(plyfile);
        } else {
            char *line = NULL;
            while (getline(&line, &n, plyfile) == -1) {
                fprintf(stderr, "Failed getline\n");
            }
            unsigned char ply = (unsigned char) atoi(line);
            free(line);
            timed = timeposition(&pos, ply);
        }

        total_time += timed;
        fprintf(stdout, "%llu,", timed);
        fflush(stdout);
    }
    fprintf(stdout, "%llu\n", total_time / 500);
    fflush(stdout);

    return(0);
}
