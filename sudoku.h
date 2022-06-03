//
// Created by ankylotech on 29.03.22.
//

#ifndef C_SUDOKU_SUDOKU_H
#define C_SUDOKU_SUDOKU_H

#include <stdio.h>
#include <math.h>


#define SUDOKU_N 3
#define SUDOKU_SIZE SUDOKU_N * SUDOKU_N

typedef struct Sudoku_ {
    int possible[SUDOKU_SIZE][SUDOKU_SIZE]
} sudoku;

int set_bit(int num, int bit);

int unset_bit(int num, int bit);

int bit(int num, int bit);

void initialize(sudoku* s, int board[SUDOKU_SIZE][SUDOKU_SIZE]);

void clear(sudoku* s);

int finishBoard(sudoku* s);

int place_number(sudoku* s, int number, int row, int column);

int logic(sudoku* sudo);

int solve_board(sudoku* s);

void output_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]);

void output_binary_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]);

void copy_sudoku(sudoku* s1,sudoku* s2);

void output_board(int s[SUDOKU_SIZE][SUDOKU_SIZE]);

#endif //C_SUDOKU_SUDOKU_H
