// readpgns.c 
// reads pgn files, and then generates matrix of evaluation data.

#undef _USE_QUIET_HASH
#define _USE_SCORE_HASH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/position.h"
#include "../src/bitscan.c"

#include "../src/position.c"
#include "../src/move.h"
#include "../src/move.c"

#include "../src/evaluation/control_eval.c"
#define _ESTIMATE_PAWN_EVAL
#include "../src/evaluation/pawn_eval.c"

#include "../src/ai.c"

#define LINESIZE 8192

// printout a line in the data matrix for evaluation
void analyze(float result, int nummoves, char *WElo, char *BElo,
	     struct position *pos)
{
	fprintf(stdout, "%.1f %d %s %s %d %d ", result, nummoves, WElo, BElo,
		pos->score, pos->tot_mat);

	char *squares = calloc(64, 1);

	// queens: rook-style attacks
	rook_control(pos, wqueens_n, squares);
	rook_control(pos, bqueens_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// queens: bishop-style attacks
	bishop_control(pos, wqueens_n, squares);
	bishop_control(pos, bqueens_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// bishop attacks
	bishop_control(pos, wbishops_n, squares);
	bishop_control(pos, bbishops_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// knight attacks
	knight_control(pos, wknights_n, squares);
	knight_control(pos, bknights_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// rook attacks
	rook_control(pos, wrooks_n, squares);
	rook_control(pos, brooks_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// pawn attacks
	pawn_control(pos, wpawns_n, squares);
	pawn_control(pos, bpawns_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// king attacks
	king_control(pos, wking_n, squares);
	king_control(pos, bking_n, squares);
	print_squares_comp(squares);
	memset(squares, 0, 64);

	// pawn evaluation:
	pawn_eval(pos);

	fprintf(stdout, "\n");
	free(squares);
}

// set the castling in mv
void user_castling(struct position *pos, struct move *mv)
{
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
}

// set the special in mv
// returns -1 on error
int user_special(struct position *pos, struct move *mv, char *buffer)
{
	mv->special = EMPTY;
	if (mv->from_p == wpawns_n) {
		if (mv->to / 8 == 3) {
			if (mv->from / 8 == 1) {
				mv->special = ADVANCE2;
			}
		} else if (mv->to / 8 == 7) {
			switch (buffer[4]) {
			case 'Q':
				mv->special = QUEEN | WHITE;
				break;
			case 'R':
				mv->special = ROOK | WHITE;
				break;
			case 'N':
				mv->special = KNIGHT | WHITE;
				break;
			case 'B':
				mv->special = BISHOP | WHITE;
				break;
			default:
				return (-1);
			}
		}
	}
	if (mv->from_p == bpawns_n) {
		if (mv->to / 8 == 4) {
			if (mv->from / 8 == 6) {
				mv->special = ADVANCE2;
			}
		} else if (mv->to / 8 == 0) {
			switch (buffer[4]) {
			case 'Q':
				mv->special = QUEEN | BLACK;
				break;
			case 'R':
				mv->special = ROOK | BLACK;
				break;
			case 'N':
				mv->special = KNIGHT | BLACK;
				break;
			case 'B':
				mv->special = BISHOP | BLACK;
				break;
			default:
				return (-1);
			}
		}
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Syntax: ./readpgns pgnfile\n");
		exit(-1);
	}
	size_t n = LINESIZE;
	char *line = calloc(n, sizeof(char));
	FILE *fp = fopen(argv[1], "r");
	int len;

	float result = -1;	//0 if black wins, 0.5 if draw, 1 if white
	char WElo[8];
	char BElo[8];
	memcpy(WElo, "NA\0\0\0\0\0\0", 8);
	memcpy(BElo, "NA\0\0\0\0\0\0", 8);

	int gamenum = 0;
	int linenum = 0;

	while ((len = getline(&line, &n, fp)) != -1) {
		if (len == 1) {	// length of one means *line == "\n"
			continue;
		} else if (strncmp(line, "[", 1) == 0) {
			if (strncmp(line, "[Result ", 8) == 0) {
				if (strncmp(line, "[Result \"0-1\"", 13) == 0) {
					result = 0;
				} else if (strncmp(line, "[Result \"1-0\"", 13)
					   == 0) {
					result = 1;
				} else
				    if (strncmp(line, "[Result \"1/2-1/2\"", 17)
					== 0) {
					result = 0.5;
				}
				// other possibility: Result = "*", detected later.
			} else if (strncmp(line, "[WhiteElo ", 8) == 0) {
				memcpy(WElo, &line[11], 4);
				if (WElo[3] == '\"') {
					WElo[3] = '\0';
					/*
					   printf("3 digit Elo\n");
					   printf("%s",line);
					   printf("%s\n",WElo);
					 */
				}
			} else if (strncmp(line, "[BlackElo ", 8) == 0) {
				memcpy(BElo, &line[11], 4);
				if (BElo[3] == '\"') {
					BElo[3] = '\0';
					/*
					   printf("3 digit Elo\n");
					   printf("%s",line);
					   printf("%s\n",BElo);
					 */
				}
			}
		} else {
			if (result == -1 || BElo[3] == '\0' || WElo[3] == '\0') {	// example, Result = "*", messed up 3-digit Elos. 3-digit Elos are all 245 or 246 or something around there, suggesting simply missing a digit
				continue;
			}
			gamenum++;

			// find how many moves it has
			int nummoves = 0;
			for (int i = 0; i < len; i++) {
				if (line[i] == ' ') {
					nummoves++;
				}
			}

			//analyze game
			char fromj, fromi, toj, toi;

			struct move *mv = malloc(sizeof(struct move));
			struct position *pos = malloc(sizeof(struct position));
			init_position(pos);

			for (int i = 0; i < len; i++) {
				if (line[i] == ' ' || line[i] == '+'
				    || line[i] == '#' || line[i] == '1'
				    || line[i] == '/' || line[i] == '0'
				    || line[i] == '2' || line[i] == '-'
				    || line[i] == '\n' || line[i] == 'Q'
				    || line[i] == 'N' || line[i] == 'B'
				    || line[i] == 'R' || line[i] == '*') {
					// stars happen maybe 7 times in allmastergames, nbd...
					/*
					   if(line[i] == '*'){
					   fprintf(stderr,"%f *\n", result);
					   }
					 */
					continue;
				} else {
					assert((line[i] >= 'a'
						&& line[i] <= 'h')
					       || (line[i] >= '1'
						   && line[i] <= 8));

					fromj = line[i + 0] - 'a';
					fromi = line[i + 1] - '1';
					toj = line[i + 2] - 'a';
					toi = line[i + 3] - '1';

					// make the move
					mv->from = fromi * 8 + fromj;
					mv->to = toi * 8 + toj;

					assert(mv->from >= 0 && mv->to >= 0);
					assert(mv->from < 64 && mv->to < 64);

					mv->from_p = find_piece(pos, mv->from);
					if ((mv->from_p == wpawns_n)
					    || (mv->from_p == bpawns_n)) {
						mv->to_p =
						    find_piece_ep(pos, mv->to);
					} else {
						mv->to_p =
						    find_piece(pos, mv->to);
					}
					user_castling(pos, mv);

					int jim =
					    user_special(pos, mv, &line[i]);
					assert(jim != -1);

					make_move(pos, mv);
					/*
					   print_position(pos);
					   char jimothy[4];
					   fscanf(stdin,"%s",jimothy);
					 */

					// do a basic quiessience tactical search to see if there is a tactic we are missing; if so, don't analyze this position because we will never use this evaluation!
					struct meta_move *mm =
					    calloc(1, sizeof(struct meta_move));
					int multiplier = pos->tomove * -2 + 1;
					quiet(pos, 0, MINUSINFINITY,
					      PLUSINFINITY, multiplier, 0, mm);

					if (abs(pos->score - mm->score) <=
					    PAWN_VAL / 2) {
						// analyze new position since no major tactics are possible
						analyze(result, nummoves, WElo,
							BElo, pos);
					}
					free(mm);

					i += 4;
				}
			}
			free(mv);
			free(pos);

			// reinitialize variables
			memcpy(WElo, "NA\0\0\0\0\0\0", 8);
			memcpy(BElo, "NA\0\0\0\0\0\0", 8);
			result = -1;

			if (gamenum % 10000 == 0) {
				fprintf(stderr, "gamenum = %d\n", gamenum);
			}
		}

		memset(line, 0, LINESIZE);
		linenum++;
	}

//      printf("finished  %d games\n", gamenum); //this fucks with reading data into R
	return (0);
}
