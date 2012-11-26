# Makefile

CC = gcc
OPTIONS = -D_SILENT
CFLAGS = -std=c99 -Wall -O3 -D_GNU_SOURCE $(OPTIONS)
LINKS = -lrt -lm

speedbenchmark:
	$(CC) $(CFLAGS) speedbenchmark.c -o speedbenchmark $(LINKS)

clean:
	rm -f speedbenchmark *.pyc
