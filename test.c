// Name:          test.c
// Usage:       ./test
// Parameters: 
// Description:
// Created On:  Sat Jun 25 14:08:18 CDT 2011
// To Compile:  $ gcc -std=c99 test.c -o test
// Author:      Naftali Harris

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define X 0.25

int x;

int sparse_popcount(uint64_t x)
{
	int count;
	for(count = 0; x; count++){
		x &= x - 1;
	}
	return count;
}

void change_x(int y)
{
	x = y;
	return;
}

int main(int argc, char **argv){
	/*
	for(int i = 0; i < 33; i++){
		printf("sparse_popcount(%d) = %d, but i = %d still.\n", i, sparse_popcount(i), i);
	}
	*/
	/*
	printf("sizeof(float) = %d\n",sizeof(float));
	for(int i = 0; i < 21; i++){
		printf("X * %d = %d\n",i, (int)(X*i));
	}
	*/

	x = 3;
	printf("x = %d\n",x);
	change_x(5);
	printf("x = %d\n",x);
        return 0;
}
