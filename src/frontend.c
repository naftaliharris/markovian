// frontend.c
// functions for interacting with the user

#include <stdio.h>
#include <string.h>

#include "move.h"
#include "frontend.h"
#include "fenio.c"

// Parses text in buffer into mv, returning an appropriate error in necessary.
// mv should already be allocated. buffer is neither freed nor altered.
// The move should begin at buffer[0].
int text2move(struct position *pos, char *buffer, struct move *mv)
{
    memset(mv, 0, sizeof(struct move));
    struct move_array *m = allmoves(pos);
    
#ifndef _XBOARD
    // castling syntax in text-mode
    if (strncmp(buffer, "O-O", 3) == 0) {
        mv->to_p = nopiece_n;
        // Queenside
        if (strncmp(buffer, "O-O-O", 5) == 0) {
            switch (pos->tomove) {
            case WHITE:
                mv->from_p = wking_n;
                mv->from = 4;
                mv->to = 2;
                mv->castling = WQUEEN;
                break;
            case BLACK:
                mv->from_p = bking_n;
                mv->from = 60;
                mv->to = 58;
                mv->castling = BQUEEN;
                break;
            }
        } 
        // Kingside
        else {
            switch (pos->tomove) {
            case WHITE:
                mv->from_p = wking_n;
                mv->from = 4;
                mv->to = 6;
                mv->castling = WKING;
                break;
            case BLACK:
                mv->from_p = bking_n;
                mv->from = 60;
                mv->to = 62;
                mv->castling = BKING;
                break;
            }
        }
    }
    else {
#endif

    // Determine to and from
    int fromj, fromi, from, toj, toi, to;
#ifdef _XBOARD
    fromj = buffer[0] - 'a';
    fromi = buffer[1] - '1';
    toj = buffer[2] - 'a';
    toi = buffer[3] - '1';
#else
    fromj = buffer[0] - 'a';
    fromi = buffer[1] - '1';
    toj = buffer[3] - 'a';
    toi = buffer[4] - '1';
#endif

    from = fromi * 8 + fromj;
    to = toi * 8 + toj;

    if(!(((0 <= from) && (from < 64)) && ((0 <= to) && (to < 64))))
    {
        free_move_array(m);
        return TXT2MV_SYNTAX_ERROR;
    }

    mv->from = (unsigned char) from;
    mv->to = (unsigned char) to;

    // Determine to_p and from_p
    mv->from_p = find_piece(pos, mv->from);
    if ((mv->from_p == wpawns_n) || (mv->from_p == bpawns_n)) {
        mv->to_p = find_piece_ep(pos, mv->to);
    } else {
        mv->to_p = find_piece(pos, mv->to);
    }

    // Determine possible castling
    mv->castling = NONE;
    if (mv->from_p == wking_n) {
        if (mv->from == 4 && mv->to == 6) {
            mv->castling = WKING;
        }
        if (mv->from == 4 && mv->to == 2) {
            mv->castling = WQUEEN;
        }
    }
    if (mv->from_p == bking_n) {
        if (mv->from == 60 && mv->to == 62) {
            mv->castling = BKING;
        }
        if (mv->from == 60 && mv->to == 58) {
            mv->castling = BQUEEN;
        }
    }

    // Determine possible special moves
    mv->special = EMPTY;
    unsigned char color;
    int promotion = 0;
    if (mv->from_p == wpawns_n) {
        if (mv->to / 8 == 3) {
            if (mv->from / 8 == 1) {
                mv->special = ADVANCE2;
            }
        } else if (mv->to / 8 == 7) {
            color = WHITE;
            promotion = 1;
        }
    }
    else if (mv->from_p == bpawns_n) {
        if (mv->to / 8 == 4) {
            if (mv->from / 8 == 6) {
                mv->special = ADVANCE2;
            }
        } else if (mv->to / 8 == 0) {
            color = BLACK;
            promotion = 1;
        }
    }

    if(promotion)
    {
#ifdef _XBOARD
        switch (buffer[4])
#else
        switch (buffer[6])
#endif
        {
        case 'q':
            mv->special = QUEEN | color;
            break;
        case 'r':
            mv->special = ROOK | color;
            break;
        case 'n':
            mv->special = KNIGHT | color;
            break;
        case 'b':
            mv->special = BISHOP | color;
            break;
        default:
            free_move_array(m);
            return (TXT2MV_PROMO_SYNTAX_ERROR);
        }
    }

#ifndef _XBOARD
    } // Close the else
#endif

    // See if the move is legal
    int found_mv = 0;
    for (int i = 0; i < m->lsize; i++) {
        if (memcmp(&m->list[i].mv, mv, sizeof(struct move)) == 0) {
            found_mv = 1;
            break;
        }
    }
    int result = found_mv ? TXT2MV_SUCCESS : TXT2MV_ILLEGAL_MV_ERROR;

    free_move_array(m);
    return result;
}

// Interact with the user
void user_input(struct position *pos)
{
    size_t bsize = 32;
    char *buffer = malloc(bsize);
    struct move *mv = malloc(sizeof(struct move));

    while (1) {
        memset(buffer, 0, bsize);
        while (getline(&buffer, &bsize, stdin) == -1) {
            fprintf(stderr, "Failed getline.\n");
            memset(buffer, 0, bsize);
        }

        if (strncmp(buffer, "quit", 4) == 0) {
            exit(0);
        }

        // text-mode options:
#ifndef _XBOARD
        // save position
        else if (strncmp(buffer, "save", 4) == 0) {
            fprintf(stdout, "File name: ");
            memset(buffer, 0, bsize);
            while (getline(&buffer, &bsize, stdin) == -1) {
                fprintf(stderr, "Failed getline.\n");
                memset(buffer, 0, bsize);
            }
            char *newline = strchr(buffer, '\n'); // remove the newline (from hitting return)
            *newline = '\0';

            FILE *fp = fopen(buffer, "w");
            char *fen = pos2fen(pos);
            fprintf(fp, "%s", fen);
            fclose(fp);
            free(fen);
        }

        // resume position
        else if (strncmp(buffer, "resume", 6) == 0) {
            fprintf(stdout, "File name: ");
            memset(buffer, 0, bsize);
            while (getline(&buffer, &bsize, stdin) == -1) {
                fprintf(stderr, "Failed getline.\n");
                memset(buffer, 0, bsize);
            }
            char *newline = strchr(buffer, '\n'); // remove the newline (from hitting return)
            *newline = '\0';

            FILE *fp = fopen(buffer, "r");
            char *fen = NULL;
            size_t n;
            while (getline(&fen, &n, fp) == -1) {
                fprintf(stderr, "Failed getline\n");
            }
            fclose(fp);
            fen2pos(pos, fen);
            free(fen);

            fprintf(stdout, "Resuming position from %s...\n", buffer);
            print_position(pos);

            if (is_check(pos)) {
                fprintf(stderr, "check\n");
            }
        }

        // enter god mode for changing board
        else if (strncmp(buffer, "I am God", 8) == 0) {
            fprintf(stdout, "All hail the Almighty Naftali!\n");
            fprintf(stdout, "Entering God mode...\n");
            while (1) {
                memset(buffer, 0, bsize);
                while (getline(&buffer, &bsize, stdin) == -1) {
                    fprintf(stderr, "Failed getline.\n");
                    memset(buffer, 0, bsize);
                }
                // exit god mode
                if (strncmp(buffer, "exit", 4) == 0) {
                    fprintf(stdout, "Exiting God mode...\n");
                    break;
                } 
                // change the position by Almighty decree
                else {
                    int flag = text2move(pos, buffer, mv);
                    switch(flag)
                    {
                        case TXT2MV_SUCCESS:
                            make_move(pos, mv);
                            print_position(pos);
                            break;
                        case TXT2MV_SYNTAX_ERROR:
                            fprintf(stderr, "Wrong syntax: Use, for example, 'e2 e4' to move the piece on e2 to e4\n");
                            break;
                        // you can still make illegal moves in God mode!
                        case TXT2MV_ILLEGAL_MV_ERROR:
                            make_move(pos, mv);
                            print_position(pos);
                            break;
                        case TXT2MV_PROMO_SYNTAX_ERROR:
                            fprintf(stderr, "Wrong syntax: Use, for example, 'e7 e8 q' to advance a pawn from e7 to e8 and promote it to a queen.\n");
                            break;
                    }
                }
            }
        }
#endif
        // xboard only options
#ifdef _XBOARD
        // xboard initialization
        else if (strncmp(buffer, "xboard", 6) == 0) {
            // pass
        }

        else if (strncmp(buffer, "protover", 8) == 0) {
            fprintf(stdout,
                "feature myname=\"Markovian\" usermove=1 sigint=0 done=1\n");
        }
#endif

        // User move command
#ifdef _XBOARD
        else if (strncmp(buffer, "usermove", 8) == 0)
#else
        else
#endif
        {
#ifdef _XBOARD
            int flag = text2move(pos, &buffer[9], mv);
#else
            int flag = text2move(pos, buffer, mv);
#endif
            switch(flag)
            {
                case TXT2MV_SUCCESS:
                    make_move(pos, mv);
                    free(mv);
                    free(buffer);
                    return;
                    break;
                case TXT2MV_SYNTAX_ERROR:
#ifdef _XBOARD
                    fprintf(stdout, "Error (ambiguous move): %s", &buffer[9]); // note: Don't add a newline since it'll already be in buffer
#else
                    fprintf(stderr, "Wrong syntax: Use, for example, 'e2 e4' to move the piece on e2 to e4\n");
#endif
                    break;
                case TXT2MV_ILLEGAL_MV_ERROR:
#ifdef _XBOARD
                    fprintf(stdout, "Illegal move: %s", &buffer[9]);
#else
                    fprintf(stderr, "Illegal move\n");
#endif
                    break;
                case TXT2MV_PROMO_SYNTAX_ERROR:
#ifdef _XBOARD
                    fprintf(stdout, "Error (ambiguous move): %s", &buffer[9]);
#else
                    fprintf(stderr, "Wrong syntax: Use, for example,\ne7 e8 q\nto advance a pawn from e7 to e8 and promote it to a queen.\n");
#endif
                    break;
            }
        }
#ifdef _XBOARD
        else
        {
            fprintf(stdout, "Error (unknown command): %s", buffer);
        }
#endif
    }
}
