// fenio_test.c
// tests for fenio.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/position.h"
#include "../src/position.h"
#include "../src/fenio.c"

int fenio_test(int verbose_errors)
{
    FILE *infile = fopen("test/data/fenio.fens", "r"); // runtests gets run from ../
    struct position pos;
    size_t n = 128 * sizeof(char);
    char *fen = malloc(n);
    char *fencopy = malloc(n);

    int failures = 0;
    for(int i = 0; i < 500; i++)
    {
        // read in the fen
        memset(fen, 0, 128);
        while (getline(&fen, &n, infile) == -1) {
            fprintf(stderr, "Failed getline\n");
        }
        assert(n == 128 * sizeof(char));

        // copy the fen
        memcpy(fencopy, fen, n);

        // move fen through fen2pos and pos2fen
        fen2pos(&pos, fen);
        char *fencopy2 = pos2fen(&pos);

        if(memcmp(fencopy, fencopy2, n) != 0)
        {
            failures++;
            if(verbose_errors)
            {
                fprintf(stderr, "fenio.c FAILURE:\n");
                fprintf(stderr, "Original FEN: %s", fencopy);
                fprintf(stderr, "Created  FEN: %s", fencopy2);
            }
        }

        free(fencopy2);
    }

    free(fen);
    free(fencopy);

    return failures;
}
