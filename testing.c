// testing.c
// simple function for testing macros and constants

void testing(void)
{
	/*
	   fprintf(stdout,"Testing Knight Moves:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(linboard[i]);
	   fprintf(stdout, "knight moves:\n");
	   print_bitboard(knightmoves[i]);
	   }
	 */

	/*
	   fprintf(stdout,"Testing Macros:\n");
	   uint64_t jim = board[1][1];
	   fprintf(stdout,"jim = \n");
	   print_bitboard(jim);
	   fprintf(stdout,"moveN(jim = \n");
	   print_bitboard(moveN(jim));
	   fprintf(stdout,"moveS(jim = \n");
	   print_bitboard(moveS(jim));
	   fprintf(stdout,"moveW(jim = \n");
	   print_bitboard(moveW(jim));
	   fprintf(stdout,"moveE(jim = \n");
	   print_bitboard(moveE(jim));
	   fprintf(stdout,"moveNW(jim = \n");
	   print_bitboard(moveNW(jim));
	   fprintf(stdout,"moveNE(jim = \n");
	   print_bitboard(moveNE(jim));
	   fprintf(stdout,"moveSE(jim = \n");
	   print_bitboard(moveSE(jim));
	   fprintf(stdout,"moveSW(jim = \n");
	   print_bitboard(moveSW(jim));
	 */

	/*
	   fprintf(stdout,"Testing Files:\n");
	   for(int i = 0; i < 8; i++){
	   fprintf(stdout,"File %d:\n",i);
	   print_bitboard(file[i]);
	   }
	   fprintf(stdout,"Testing Ranks:\n");
	   for(int i = 0; i < 8; i++){
	   fprintf(stdout,"Rank %d:\n",i);
	   print_bitboard(rank[i]);
	   }
	 */

	/*
	   fprintf(stdout,"Testing Rook Moves:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(rookmoves[i]);
	   }

	   fprintf(stdout,"Testing Bishop Moves:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(bishopmoves[i]);
	   }
	 */

	/*
	   fprintf(stdout,"Testing Rook Masks:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(RookMasks[i]);
	   }

	   fprintf(stdout,"Testing Bishop Masks:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(BishopMasks[i]);
	   }

	   fprintf(stdout,"Testing Knight_Castle Masks:\n");
	   for(int i = 0; i < 4; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(Knight_Castling[i]);
	   }
	   fprintf(stdout,"Testing Pawn_Castle Masks:\n");
	   for(int i = 0; i < 4; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(Pawn_Castling[i]);
	   }
	   fprintf(stdout,"Testing notlinboard Masks:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(notlinboard[i]);
	   }
	   fprintf(stdout,"Testing white pawn_attacks Masks:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(pawn_attacks[0][i]);
	   }
	   fprintf(stdout,"Testing black pawn_attacks Masks:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(pawn_attacks[1][i]);
	   }
	   fprintf(stdout,"Testing vectorN:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(vectorN[i]);
	   }
	   fprintf(stdout,"Testing vectorS:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(vectorS[i]);
	   }
	   fprintf(stdout,"Testing vectorE:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(vectorE[i]);
	   }
	   fprintf(stdout,"Testing vectorW:\n");
	   for(int i = 0; i < 64; i++){
	   fprintf(stdout, "i = %d:\n",i);
	   print_bitboard(vectorW[i]);
	   }
	   for(char i = 0; i < 64; i++){
	   print_bitboard(linboard[i]);
	   print_bitboard(linboard[flip(i)]);
	   }
	 */

	fprintf(stdout, "MINUSINFINITY = %d\n", MINUSINFINITY);
	fprintf(stdout, "PLUSINFINITY = %d\n", PLUSINFINITY);
	fprintf(stdout, "TOLERANCE = %d\n", TOLERANCE);
	fprintf(stdout, "TOLERANCE/100 = %d\n", TOLERANCE / 100);
	fprintf(stdout, "MATCUTOFF = %d\n", MATCUTOFF);
	fprintf(stdout, "ASPIRATION_WINDOW = %d\n", ASPIRATION_WINDOW);
	fprintf(stdout, "KING_CAPTURED_SCORE = %d\n", KING_CAPTURED_SCORE);
	fprintf(stdout, "ROOK_VAL = %d\n", ROOK_VAL);
	fprintf(stdout, "KNIGHT_VAL = %d\n", KNIGHT_VAL);
	fprintf(stdout, "BISHOP_VAL = %d\n", BISHOP_VAL);
	fprintf(stdout, "CONTEMPT = %d\n", CONTEMPT);

}
