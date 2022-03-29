#include "sudoku.h"
#include <stdio.h>
#include<stdlib.h>
#include <sys/time.h>

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
#include "sudoku.h"
int main(){
    FILE* input = fopen("./input.txt","r");
    struct timeval t0;
    struct timeval t1;
    float elapsed;
    int board[SUDOKU_SIZE][SUDOKU_SIZE]= {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 9, 5, 3, 0},
            {8, 0, 9, 0, 7, 6, 0, 0, 0},
            {9, 0, 0, 0, 1, 0, 4, 0, 0},
            {0, 1, 4, 0, 9, 7, 0, 0, 3},
            {6, 0, 0, 0, 8, 0, 2, 0, 0},
            {4, 0, 7, 0, 5, 3, 0, 0, 0},
            {0, 0, 2, 0, 0, 8, 7, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    if(input != NULL){
        for(int i = 0; i < SUDOKU_SIZE; i++){
            for(int j = 0; j < SUDOKU_SIZE; j++){
                board[i][j] = fgetc(input) - '0';
            }
            fgetc(input);
        }
    }
    sudoku s;
    output_matrix(board);
    gettimeofday(&t0,0);
    initialize(&s,board);
    solve_board(&s);
    gettimeofday(&t1,0);

    elapsed = timedifference_msec(t0, t1);

    printf("\nsolved:\n\n");
    output_matrix(s.board);
    printf("solved in %fms", elapsed);
    return 0;
}