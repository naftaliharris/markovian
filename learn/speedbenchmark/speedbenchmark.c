// speedbenchmark.c
// a routine for testing Markovian on speedbenchmark.fens

#define __USE_STRING_INLINES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>

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
    fprintf(stderr, "Usage: %s [--new-plies seconds] [--fen-file fenfile] [--ply-file plyfile] [--average-only]\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    unsigned long long over_time = 0;
    char *fenfilename = "speedbenchmark.fens";
    char *plyfilename = "speedbenchmark.plies";
    int averageonly = 0;

    // Parse the arguments
    int c;
    while (1) {

        int option_index = 0;

        static struct option long_options[] = {
            {"new-plies", 1, 0, 0},
            {"fen-file", 1, 0, 0},
            {"ply-file", 1, 0, 0},
            {"average-only", 0, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 0:
            switch(option_index) {
            case 0:
                over_time = (unsigned long long) (BILLION * atof(optarg));
                break;
            case 1:
                fenfilename = optarg;
                break;
            case 2:
                plyfilename = optarg;
                break;
            case 3:
                averageonly = 1;
                break;
            }
            break;

        default:
            usage(argv[0]);
            break;
        }
    }

    if(optind < argc) {
        usage(argv[0]);
    }

    struct position pos;
    FILE *fenfile = fopen(fenfilename, "r");
    FILE *plyfile = (over_time == 0) ? fopen(plyfilename, "r") : fopen(plyfilename, "w");
    char *fen;
    size_t n;
    unsigned long long total_time = 0;

    int fen_count;
    for(fen_count = 0; ; fen_count++)
    {
        fen = NULL;
        if (getline(&fen, &n, fenfile) == -1) {
            // EOL
            break;
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
            if (getline(&line, &n, plyfile) == -1) {
                fprintf(stderr, "Failed getline\n");
                exit(EXIT_FAILURE);
            }
            unsigned char ply = (unsigned char) atoi(line);
            free(line);
            timed = timeposition(&pos, ply);
        }

        total_time += timed;
        if(!averageonly) {
            fprintf(stdout, "%llu,", timed);
            fflush(stdout);
        }
    }

    fprintf(stdout, "%llu\n", total_time / fen_count);
    fflush(stdout);

    return(0);
}
