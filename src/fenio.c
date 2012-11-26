// fenio.c
// converts between .fen format and the position struct

#ifndef _FENIO_C
#define _FENIO_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "position.h"
#include "position.c"
#include "bitboard.h"

// reads a fen string into an alloc'd position struct
// destroys the fen string, no freeing done
// XXX: Need to take care of the history list externally.
void fen2pos(struct position *pos, char *fen)
{
    // Clear the position
    memset(pos, 0, sizeof(struct position));

    // Figure out piece placements
    char *space = " ";
    char *saveptr1;
    char *pieces = strtok_r(fen, space, &saveptr1);

    char *slash = "/";
    char *saveptr2;
    char *pieces_temp = pieces;
    for(int i = 7; i >= 0; i--)
    {
        char *row = strtok_r(pieces_temp, slash, &saveptr2);
        int j = 0;
        for(int k = 0; row[k] != '\0'; k++)
        {
            char c = row[k];
            switch(c)
            {
                case 'K':
                    pos->wking |= board[i][j];
                    pos->score += KING_VAL;
                    j += 1;
                    break;
                case 'k':
                    pos->bking |= board[i][j];
                    pos->score -= KING_VAL;
                    j += 1;
                    break;
                case 'Q':
                    pos->wqueens |= board[i][j];
                    pos->score += QUEEN_VAL;
                    pos->tot_mat += QUEEN_VAL;
                    j += 1;
                    break;
                case 'q':
                    pos->bqueens |= board[i][j];
                    pos->score -= QUEEN_VAL;
                    pos->tot_mat += QUEEN_VAL;
                    j += 1;
                    break;
                case 'R':
                    pos->wrooks |= board[i][j];
                    pos->score += ROOK_VAL;
                    pos->tot_mat += ROOK_VAL;
                    j += 1;
                    break;
                case 'r':
                    pos->brooks |= board[i][j];
                    pos->score -= ROOK_VAL;
                    pos->tot_mat += ROOK_VAL;
                    j += 1;
                    break;
                case 'B':
                    pos->wbishops |= board[i][j];
                    pos->score += BISHOP_VAL;
                    pos->tot_mat += BISHOP_VAL;
                    j += 1;
                    break;
                case 'b':
                    pos->bbishops |= board[i][j];
                    pos->score -= BISHOP_VAL;
                    pos->tot_mat += BISHOP_VAL;
                    j += 1;
                    break;
                case 'N':
                    pos->wknights |= board[i][j];
                    pos->score += KNIGHT_VAL;
                    pos->tot_mat += KNIGHT_VAL;
                    j += 1;
                    break;
                case 'n':
                    pos->bknights |= board[i][j];
                    pos->score -= KNIGHT_VAL;
                    pos->tot_mat += KNIGHT_VAL;
                    j += 1;
                    break;
                case 'P':
                    pos->wpawns |= board[i][j];
                    pos->score += PAWN_VAL;
                    pos->tot_mat += PAWN_VAL;
                    j += 1;
                    break;
                case 'p':
                    pos->bpawns |= board[i][j];
                    pos->score -= PAWN_VAL;
                    pos->tot_mat += PAWN_VAL;
                    j += 1;
                    break;
                default: // a number
                    j += (c - '1' + 1);
                    break;
            }
        }
        pieces_temp = NULL;
    }

    // Figure out who is to move
    char *tomove = strtok_r(NULL, space, &saveptr1);
    switch(tomove[0])
    {
        case 'w':
            pos->tomove = WHITE;
            pos->towait = BLACK;
            break;
        default:
            pos->tomove = BLACK;
            pos->towait = WHITE;
            break;
    }

    // Figure out castling
    pos->perm_castling = NONE;
    char *castling = strtok_r(NULL, space, &saveptr1);
    for(int k = 0; castling[k] != '\0'; k++)
    {
        char c = castling[k];
        switch(c)
        {
            case 'K':
                pos->perm_castling &= ~WKING;
                break;
            case 'k':
                pos->perm_castling &= ~BKING;
                break;
            case 'Q':
                pos->perm_castling &= ~WQUEEN;
                break;
            case 'q':
                pos->perm_castling &= ~BQUEEN;
                break;
        }
    }

    // Figure out en passant
    char *enpassant = strtok_r(NULL, space, &saveptr1);
    switch(enpassant[0])
    {
        case '-':
            pos->ep = EP_NONE;
            break;
        default:
            pos->ep = enpassant[0] - 'a';
            break;
    }

    // Figure out halfmove clock
    char *halfmove = strtok_r(NULL, space, &saveptr1);
    int half_move_clock = atoi(halfmove);
    pos->half_move_clock = (signed char) half_move_clock;

    // Figure out fullmove number, remembering that FEN has 1-indexing
    char *fullmove = strtok_r(NULL, space, &saveptr1);
    int fullmoves = atoi(fullmove);
    pos->move_num = (uint16_t) (2 * (fullmoves - 1) + pos->tomove);

    // Compute and impute the rest of the position
	pos->wpieces =
	    pos->wrooks | pos->wknights | pos->wbishops | pos->wqueens | pos->wking | pos->wpawns;
	pos->bpieces =
	    pos->brooks | pos->bknights | pos->bbishops | pos->bqueens | pos->bking | pos->bpawns;
	pos->allpieces = pos->wpieces | pos->bpieces;

    pos->limp1 = 0; // limp1 is used to determine repetitions, so set this to zero to be safe.

	pos->hash = make_hash(pos);

    assert(consistency(pos));
}

const char fenpieces[12] = {'K', 'k', 'Q', 'q', 'R', 'r', 'B', 'b', 'N', 'n', 'P', 'p'};

// returns a fen string representing the position pos
char *pos2fen(struct position *pos)
{
    assert(consistency(pos));

    char *fen = calloc(128, sizeof(char)); // FEN strings are always shorter than ~85 characters
    int k = 0; // the write location

    // Figure out piece placements
    for(int i = 7; i >= 0; i--)
    {
        int blanks = 0;
        for(int j = 0; j < 8; j++)
        {
            for(int l = 0; l < 12; l++)
            {
                if(pos->pieces[l] & board[i][j]) {
                    if(blanks > 0)
                    {
                        fen[k++] = (char) ('0' + blanks);
                    }
                    fen[k++] = fenpieces[l];
                    blanks = -1;
                    break;
                }
            }
            blanks++;
        }
        if(blanks > 0)
        {
            fen[k++] = (char) ('0' + blanks);
        }
        if(i > 0)
        {
            fen[k++] = '/';
        }
    }
    fen[k++] = ' ';

    // Figure out who is to move
    fen[k++] = (pos->tomove == WHITE) ? 'w' : 'b';
    fen[k++] = ' ';

    // Figure out castling rights
    if(pos->perm_castling == NONE)
    {
        fen[k++] = '-';
    }
    else
    {
        if(!(pos->perm_castling & WKING))
            fen[k++] = 'K';
        if(!(pos->perm_castling & WQUEEN))
            fen[k++] = 'Q';
        if(!(pos->perm_castling & BKING))
            fen[k++] = 'k';
        if(!(pos->perm_castling & BQUEEN))
            fen[k++] = 'q';
    }
    fen[k++] = ' ';

    // Figure out en passant
    if(pos->ep == EP_NONE)
    {
        fen[k++] = '-';
    }
    else
    {
        fen[k++] = (char) ('a' + pos->ep);
        fen[k++] = (pos->towait == WHITE) ? '3' : '6';
    }
    fen[k++] = ' ';

    // Figure out halfmove clock
    k += sprintf(&fen[k], "%d ", pos->half_move_clock);

    // Figure out fullmove number, remembering that FEN has 1-indexing
    sprintf(&fen[k], "%d\n", pos->move_num / 2 + 1);

    return fen;
}

#endif
