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
    FILE* file = NULL;
    file = fopen("C:/Users/Ankylotech/CLionProjects/C_Sudoku/puzzles/allpuzzles.txt","r");
    int repetitions = 1;
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
    if(file != NULL) {
        repetitions = readNumberLine(file);
        printf("%d boards\n",repetitions);
    }
    int solveTotal = 0;
    float max = 0;
    int maxI = 0;
    float min = 100000;
    int minI = 0;
    for(int i = 0; i < repetitions; i++) {
        if (file != NULL) {
            for (int x = 0; x < SUDOKU_SIZE; x++) {
                for (int y = 0; y < SUDOKU_SIZE; y++) {
                    board[x][y] = readSingleNumber(file);
                }
            }
        }
        //printf("board %d:\n",(i+1));
        gettimeofday(&t0,0);
        initialize(&s,board);
        //output_matrix(s.possible);
        if(!solve_board(&s)){
            output_binary_matrix(s.possible);
            printf("impossible\n");
        }

        gettimeofday(&t1,0);
        if(finishBoard(&s)) {
            solveTotal++;
            //printf("solved:\n");
        }else printf("failed\n");
        //output_matrix(s.possible);
        float d = timedifference_msec(t0, t1);
        if(d > max) {
            max = d;
            maxI = i+1;
        }
        if(d < min) {
            min = d;
            minI = i;
        }
        elapsed += d;

        //printf("finished board %d\n",(i+1));
    }
    printf("solved %d of %d in %fms avg: %fms / board, max: %fms at %d, min: %fms at %d",solveTotal, repetitions, elapsed, elapsed/repetitions, max, maxI, min, minI);
    return 0;
}