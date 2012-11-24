// frontend.c
// functions for interacting with the user

#include <stdio.h>
#include <string.h>

#include "move.h"
#include "fenio.c"

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
#ifdef _XBOARD
			switch (buffer[13])
#else
			switch (buffer[6])
#endif
			{
			case 'q':
				mv->special = QUEEN | WHITE;
				break;
			case 'r':
				mv->special = ROOK | WHITE;
				break;
			case 'n':
				mv->special = KNIGHT | WHITE;
				break;
			case 'b':
				mv->special = BISHOP | WHITE;
				break;
			default:
				fprintf(stderr,
					"Promotion syntax example: e7 e8 q\n");
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
#ifdef _XBOARD
			switch (buffer[13])
#else
			switch (buffer[6])
#endif
			{
			case 'q':
				mv->special = QUEEN | BLACK;
				break;
			case 'r':
				mv->special = ROOK | BLACK;
				break;
			case 'n':
				mv->special = KNIGHT | BLACK;
				break;
			case 'b':
				mv->special = BISHOP | BLACK;
				break;
			default:
				fprintf(stderr,
					"Promotion syntax example: e2 e1 q\n");
				return (-1);
			}
		}
	}
	return (0);
}

// no castling or promotion yet
void user_move(struct position *pos)
{
	char fromj, fromi, toj, toi;
	size_t bsize = 32;
	char *buffer = malloc(bsize);
	char done = 0;
	struct move *mv = malloc(sizeof(struct move));
	struct move_array *m = allmoves(pos);

	while (!done) {
		unsigned char p = nopiece_n;
		while (p == nopiece_n) {
			memset(buffer, 0, bsize);
			if (getline(&buffer, &bsize, stdin) == -1) {
				fprintf(stderr, "Failed getline.\n");
			}
#ifndef _XBOARD
			// save position
			if (strncmp(buffer, "save", 4) == 0) {
				fprintf(stdout, "File name: ");
				memset(buffer, 0, bsize);
				if (getline(&buffer, &bsize, stdin) == -1) {
					fprintf(stderr, "Failed getline.\n");
				}
				//remove the newline (from hitting return)
				char *newline = strchr(buffer, '\n');
				*newline = '\0';

				FILE *fp = fopen(buffer, "w");
				fwrite(pos, sizeof(struct position), 1, fp);
				fclose(fp);
			}
			// resume position
			else if (strncmp(buffer, "resume", 6) == 0) {
				fprintf(stdout, "File name: ");
				memset(buffer, 0, bsize);
				if (getline(&buffer, &bsize, stdin) == -1) {
					fprintf(stderr, "Failed getline.\n");
				}
				//remove the newline (from hitting return)
				char *newline = strchr(buffer, '\n');
				*newline = '\0';

				FILE *fp = fopen(buffer, "r");
                char *fen = NULL;
                size_t n;
				if (getline(&fen, &n, fp) == -1) {
					fprintf(stderr, "Failed getline\n");
				}
				fclose(fp);
                fen2pos(pos, fen);
                free(fen);

				fprintf(stdout,
					"Resuming position from %s...\n",
					buffer);
				free_move_array(m);
				m = allmoves(pos);
				print_position(pos);

				if (is_check(pos)) {
					fprintf(stderr, "check\n");
				}
			}
			// enter god mode for changing board
			else if (strncmp(buffer, "I am God", 8) == 0) {
				fprintf(stdout,
					"All hail the Allmighty Naftali!\n");
				fprintf(stdout, "Entering God mode...\n");
				unsigned char done = 0;
				while (!done) {
					memset(buffer, 0, bsize);
					if (getline(&buffer, &bsize, stdin) ==
					    -1) {
						fprintf(stderr,
							"Failed getline.\n");
					}
					if (strncmp(buffer, "exit", 4) == 0) {
						fprintf(stdout,
							"Exiting God mode...\n");
						free_move_array(m);
						m = allmoves(pos);
						done = 1;
					} else {
						fromj = buffer[0] - 'a';
						fromi = buffer[1] - '1';
						toj = buffer[3] - 'a';
						toi = buffer[4] - '1';

						mv->from = fromi * 8 + fromj;
						mv->to = toi * 8 + toj;
						mv->from_p =
						    find_piece(pos, mv->from);

						if ((mv->from_p == wpawns_n)
						    || (mv->from_p ==
							bpawns_n)) {
							mv->to_p =
							    find_piece_ep(pos,
									  mv->
									  to);
						} else {
							mv->to_p =
							    find_piece(pos,
								       mv->to);
						}

						user_castling(pos, mv);
						if (user_special
						    (pos, mv, buffer) == -1) {
							p = nopiece_n;
						} else {
							p = mv->from_p;
						}
						if (p != nopiece_n) {
							make_move(pos, mv);
							print_position(pos);
						}
					}
				}
			}
#endif

			// castling
#ifdef _XBOARD
			if (strncmp(buffer, "O-O", 3) == 0)
#else
			else if (strncmp(buffer, "O-O", 3) == 0)
#endif
			{
				if (strncmp(buffer, "O-O-O", 5) == 0) {
					switch (pos->tomove) {
					case WHITE:
						mv->castling = WQUEEN;
						break;
					case BLACK:
						mv->castling = BQUEEN;
						break;
					}
				} else {
					switch (pos->tomove) {
					case WHITE:
						mv->castling = WKING;
						break;
					case BLACK:
						mv->castling = BKING;
						break;
					}
				}
				make_move(pos, mv);

				//free_move_array(m);
				free(mv);
				free(buffer);
				return;
			}
#ifdef _XBOARD
			// xboard initialization
			else if (strncmp(buffer, "xboard", 6) == 0) {
			}

			else if (strncmp(buffer, "protover", 8) == 0) {
				fprintf(stdout,
					"feature myname=\"Markovian\" usermove=1 sigint=0 done=1\n");
			}
#endif

			else if (strncmp(buffer, "quit", 4) == 0) {
				exit(0);
			}
			// actually move
#ifdef _XBOARD
			else if (strncmp(buffer, "usermove", 8) == 0)
#else
			else
#endif
			{

#ifndef _XBOARD
				fromj = buffer[0] - 'a';
				fromi = buffer[1] - '1';
				toj = buffer[3] - 'a';
				toi = buffer[4] - '1';
#endif
#ifdef _XBOARD
				fromj = buffer[9] - 'a';
				fromi = buffer[10] - '1';
				toj = buffer[11] - 'a';
				toi = buffer[12] - '1';
#endif
				mv->from = fromi * 8 + fromj;
				mv->to = toi * 8 + toj;
				mv->from_p = find_piece(pos, mv->from);
				if ((mv->from_p == wpawns_n)
				    || (mv->from_p == bpawns_n)) {
					mv->to_p = find_piece_ep(pos, mv->to);
				} else {
					mv->to_p = find_piece(pos, mv->to);
				}
				user_castling(pos, mv);

				if (user_special(pos, mv, buffer) == -1) {
					p = nopiece_n;
				} else {
					p = mv->from_p;
				}
			}
		}
		//find move
		for (int i = 0; i < m->lsize; i++) {
			if (memcmp(&m->list[i].mv, mv, sizeof(struct move)) ==
			    0) {
				done = 1;
				break;
			}
		}
		if (!done) {
			fprintf(stdout, "Illegal move\n");
		}
	}

	make_move(pos, mv);

	free_move_array(m);
	free(mv);
	free(buffer);
}
