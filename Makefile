# Makefile
# Notes:	Add -D_XBOARD to CFLAGS to compile version for linking to xboard, (run as $ xboard -fcp ./chess -fp <directory of ./chess>)
#		Add -g -D_DEBUG to CFLAGS to turn on debugging

CC = gcc
SRC = src
DEPS = $(SRC)/chess.c $(SRC)/move.c $(SRC)/makemove.c $(SRC)/move.h $(SRC)/position.c $(SRC)/position.h $(SRC)/frontend.c $(SRC)/ai.c $(SRC)/timer.c $(SRC)/allmoves.c $(SRC)/testing.c $(SRC)/evaluate.c $(SRC)/bitscan.c $(SRC)/evaluation/control_eval.c $(SRC)/hash/hash.c $(SRC)/evaluation/pawn_eval.c $(SRC)/features.h $(SRC)/history.h $(SRC)/history.c Makefile 
OPTIONS = -D_USE_HISTORY
CFLAGS = -std=c99 -Wall -D_GNU_SOURCE $(OPTIONS)
LINKS = -lrt -lm
MAIN = $(SRC)/chess.c

xboard: $(DEPS)
	$(CC) $(CFLAGS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT $(MAIN) -o markovian $(LINKS)
debug: $(DEPS)
	$(CC) $(CFLAGS) -g -D_DEBUG $(MAIN) -o markovian $(LINKS)
testing: $(DEPS)
	$(CC) $(CFLAGS) -O3 $(MAIN) -o markovian $(LINKS)
profile: $(DEPS)
	$(CC) $(CFLAGS) -03 -fno-inline -g -pg $(MAIN) -o markovian $(LINKS)
cache-hits: $(DEPS)
	$(CC) $(CFLAGS) -g -D_NODE_COUNT -D_CACHE_HITS $(MAIN) -o markovian $(LINKS)
install: $(DEPS)
	$(CC) $(CFLAGS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT $(MAIN) -o markovian $(LINKS)
	mv markovian /usr/bin
clean:
	rm -f markovian *.o a.out gmon.out
