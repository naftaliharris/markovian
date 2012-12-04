// squares.h
// functions for dealing with character arrays of size 64

#ifndef _SQUARES_H
#define _SQUARES_H

// for humans
void print_squares(char *squares)
{
    fprintf(stdout,"---------------------------------\n");
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            int n = i*8 + j;
            if(squares[n] >= 0){
                fprintf(stdout,"| %d ", squares[n]);
            }
            else {
                fprintf(stdout,"| %d", squares[n]); //no space after to handle the minus sign properly
            }
        }
        fprintf(stdout,"|\n");
        fprintf(stdout,"---------------------------------\n");
    }
}

// for computers (ie, for printing data to evaluate)
void print_squares_comp(char *squares)
{
    for(int i = 0; i < 64; i++){
        fprintf(stdout,"%d ",squares[i]);
    }
}

#endif
