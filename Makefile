# Makefile
# Notes:	Add -D_XBOARD to CFLAGS to compile version for linking to xboard, (run as $ xboard -fcp ./chess -fp <directory of ./chess>)
#		Add -g -D_DEBUG to CFLAGS to turn on debugging

CC = gcc
DEPS = chess.c move.c makemove.c move.h position.c position.h frontend.c ai.c timer.c allmoves.c testing.c evaluate.c bitscan.c evaluation/control_eval.c hash/hash.c evaluation/pawn_eval.c features.h history.h history.c Makefile 
CFLAGS = -std=c99 -Wall -D_GNU_SOURCE
LINKS = -lrt -lm

xboard: $(DEPS)
	$(CC) $(CFLAGS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT chess.c -o markovian $(LINKS)
debug: $(DEPS)
	$(CC) $(CFLAGS) -g -D_DEBUG chess.c -o markovian $(LINKS)
testing: $(DEPS)
	$(CC) $(CFLAGS) -O3 chess.c -o markovian $(LINKS)
profile: $(DEPS)
	$(CC) $(CFLAGS) -03 -fno-inline -g -pg chess.c -o markovian $(LINKS)
cache-hits: $(DEPS)
	$(CC) $(CFLAGS) -g -D_NODE_COUNT -D_CACHE_HITS chess.c -o markovian $(LINKS)
install: $(DEPS)
	$(CC) $(CFLAGS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT chess.c -o markovian $(LINKS)
	mv markovian /usr/bin
clean:
	rm -f markovian *.o a.out gmon.out
