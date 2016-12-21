# Makefile

CC = gcc
SRC = src
TEST = test
SRCDEPS = $(SRC)/chess.c $(SRC)/move.c $(SRC)/makemove.c $(SRC)/move.h $(SRC)/position.c $(SRC)/position.h $(SRC)/frontend.c $(SRC)/ai.c $(SRC)/timer.c $(SRC)/allmoves.c $(SRC)/testing.c $(SRC)/evaluate.c $(SRC)/bitscan.c $(SRC)/evaluation/control_eval.c $(SRC)/hash/hash.c $(SRC)/evaluation/pawn_eval.c $(SRC)/features.h $(SRC)/history.h $(SRC)/history.c Makefile 
TESTDEPS = $(TEST)/test.c $(TEST)/fenio_test.c $(TEST)/search_test.c
PLAY_OPTIONS = -D_USE_HISTORY -D_USE_ASPIRATION_SEARCH
CFLAGS = -std=c99 -Wall -D_GNU_SOURCE
LINKS = -lm
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LINKS += -lrt
endif
MAIN = $(SRC)/chess.c

# Compile version for use by xboard
xboard: $(SRCDEPS)
	$(CC) $(CFLAGS) $(PLAY_OPTIONS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT $(MAIN) -o markovian $(LINKS)

# Compile version with text interface, for use in debugging
debug: $(SRCDEPS)
	$(CC) $(CFLAGS) $(PLAY_OPTIONS) -g -D_DEBUG $(MAIN) -o markovian $(LINKS)

# Compile version for profiling code
profile: $(SRCDEPS)
	$(CC) $(CFLAGS) $(PLAY_OPTIONS) -O3 -fno-inline -g -pg $(MAIN) -o markovian $(LINKS)

# Compile version for measuring behavior of transposition tables
cache-hits: $(SRCDEPS)
	$(CC) $(CFLAGS) $(PLAY_OPTIONS) -O3 -D_NODE_COUNT -D_CACHE_HITS $(MAIN) -o markovian $(LINKS)

# Compile xboard and install it in /usr/bin
install: $(SRCDEPS)
	$(CC) $(CFLAGS) $(PLAY_OPTIONS) -O3 -D_XBOARD -D_IT_DEP -D_NODE_COUNT $(MAIN) -o markovian $(LINKS)
	mv markovian /usr/bin

# Clean up the binaries
clean:
	rm -f markovian unittest aspire_search no_aspire_search *.o a.out gmon.out

###################################
# Binaries for use by in test.py: #
###################################

# Compile unittests
unittest: $(SRCDEPS) $(TESTDEPS)
	$(CC) $(CFLAGS) -g -D_DEBUG -D_SILENT $(TEST)/test.c -o unittest $(LINKS)

# Aspiration search
aspire_search: $(SRCDEPS) $(TESTDEPS)
	$(CC) $(CFLAGS) -O3 -D_USE_ASPIRATION_SEARCH -D_SILENT $(TEST)/search_test.c -o aspire_search $(LINKS)

# Ordinary search
no_aspire_search: $(SRCDEPS) $(TESTDEPS)
	$(CC) $(CFLAGS) -D_SILENT $(TEST)/search_test.c -o no_aspire_search $(LINKS)
