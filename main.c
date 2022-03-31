#include "sudoku.h"
#include "io.h"
#include <stdio.h>
#include <sys/time.h>

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
int main(){
    struct timeval t0;
    struct timeval t1;
    FILE* file = fopen("C:/Users/Ankylotech/CLionProjects/C_Sudoku/input.txt","r");
    int repetitions = 1000;
    float elapsed = 0;
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
    sudoku s;
    output_matrix(board);
    if(file != NULL) {
        repetitions = readNumberLine(file);
        printf("%d boards\n",repetitions);
    }
    for(int i = 0; i < repetitions; i++){
        for(int x = 0; x < SUDOKU_SIZE; x++){
            for(int y = 0; y < SUDOKU_SIZE; y++){
                board[x][y] = readSingleNumber(file);
            }
        }
        printf("board %d:\n",(i+1));
        output_matrix(board);
        gettimeofday(&t0,0);

        initialize(&s,board);
        solve_board(&s);
        gettimeofday(&t1,0);
        printf("solved:\n");
        output_matrix(s.board);
        elapsed += timedifference_msec(t0, t1);
        printf("finished board %d\n",(i+1));
    }


    printf("\nsolved:\n\n");
    output_matrix(s.board);
    printf("solved %d in %fms avg: %fms / board", repetitions, elapsed, elapsed/repetitions);
    return 0;
}