# Makefile

CC = gcc
OPTIONS = -D_SILENT -D_USE_ASPIRATION_SEARCH
CFLAGS = -std=c99 -Wall -O3 -D_GNU_SOURCE $(OPTIONS)
LINKS = -lrt -lm

speedbenchmark:
	$(CC) $(CFLAGS) speedbenchmark.c -o a.out $(LINKS)

clean:
	rm -f a.out
