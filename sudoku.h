//
// Created by ankylotech on 29.03.22.
//

#ifndef C_SUDOKU_SUDOKU_H
#define C_SUDOKU_SUDOKU_H

#include <stdio.h>
#include <math.h>

#define SUDOKU_SIZE 9
#define ROW_SIZE 3
#define COLUMN_SIZE 3



typedef struct Sudoku_ {
    int board[SUDOKU_SIZE][SUDOKU_SIZE];
    int possible[SUDOKU_SIZE][SUDOKU_SIZE]
} sudoku;

int set_bit(int num, int bit);

int unset_bit(int num, int bit);

int bit(int num, int bit);

void initialize(sudoku* s, int board[SUDOKU_SIZE][SUDOKU_SIZE]);

int place_number(sudoku* s, int number, int row, int column);

int solve_board(sudoku* s);

void output_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]);

void copy_sudoku(sudoku* s1,sudoku* s2);

#endif //C_SUDOKU_SUDOKU_H
