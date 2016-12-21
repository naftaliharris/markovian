// chess.c
// a high performance chess engine written by Naftali

#include "features.h"

#define __USE_STRING_INLINES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#ifndef _DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include "bitscan.c"
#include "position.h"
#include "move.h"
#include "history.h"
#include "history.c"

#include "move.c"
#include "hash/hash.c"
#include "position.c"
#include "addmove.c"
#include "allmoves.c"
#include "makemove.c"
#include "frontend.c"
#include "evaluate.c"
#include "timer.c"
#include "ai.c"
#include "testing.c"

void INThandler(int sig)
{
    exit(-1);
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    signal(SIGINT, INThandler);

    unsigned char ply = 6;

    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            ply = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-p ply]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    struct position pos;
    init_position(&pos);
    assert(consistency(&pos));

    struct timespec realt_old, realt_new;

    tt = new_trans_tables();

#ifdef _USE_HISTORY
    hist = init_history();
#endif

#ifdef _DEBUG
    testing();
#endif

#ifndef _XBOARD
    print_position(&pos);
#endif

    while (1) {
        if (is_check(&pos)) {
            fprintf(stderr, "check.\n");
        }

        user_input(&pos);
#ifndef _XBOARD
        print_position(&pos);
#endif
        assert(consistency(&pos));

        if (is_check(&pos)) {
            fprintf(stderr, "check.\n");
        }

        current_utc_time(&realt_old);
        computer_move(&pos, ply);
        assert(consistency(&pos));
        current_utc_time(&realt_new);

#ifndef _XBOARD
        fprintf(stderr, "One second: %12llu ns\n",
            (long long unsigned)BILLION);
        fprintf(stderr, "Real  time: %12llu ns\n",
            (long long unsigned)minus_time(&realt_new, &realt_old));

        print_position(&pos);
#endif
    }

    return (0);
}
