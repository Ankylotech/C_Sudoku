//
// Created by ankylotech on 29.03.22.
//
#include "sudoku.h"

int set_bit(int num, int bit){
    return (num | (1<<bit));
}

int unset_bit(int num, int bit){
    return (num & (~(1<<bit)));
}

int bit(int num, int bit){
    return (num >> bit) & 1;
}

void copy_sudoku(sudoku* s1,sudoku* s2){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            s1->board[i][j] = s2->board[i][j];
            s1->possible[i][j] = s2->possible[i][j];
        }
    }
}

void initialize(sudoku* s, int board[SUDOKU_SIZE][SUDOKU_SIZE]){
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            for(int i = 1; i <= SUDOKU_SIZE; i++){
                s->possible[row][column] = set_bit(s->possible[row][column],i);
            }

        }
    }
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            place_number(s,board[row][column],row,column);
        }
    }
}

int num_bits(int n){
    int tot = 0;
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) tot++;
    }
    return tot;
}

int first_bit(int n){
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) return i;
    }
    return 0;
}

int place_number(sudoku* s, int number, int row, int column){
    s->board[row][column] = number;
    if(number == 0) return 1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        s->possible[row][i] = unset_bit(s->possible[row][i],number);
        s->possible[i][column] = unset_bit(s->possible[i][column],number);

    }
    int rowBox = row - row%ROW_SIZE;
    int columnBox = column - column%COLUMN_SIZE;
    for(int i = rowBox; i < rowBox + ROW_SIZE; i++){
        for(int j = columnBox; j < columnBox + COLUMN_SIZE; j++){
            s->possible[i][j] = unset_bit(s->possible[i][j],number);
        }
    }
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(s->board[i][column] == 0 && num_bits(s->possible[i][column]) == 1){
            place_number(s, first_bit(s->possible[i][column]),i,column);
        }
        if(s->board[row][i] == 0 && num_bits(s->possible[row][i]) == 1){
            place_number(s, first_bit(s->possible[row][i]),row,i);
        }
    }
    for(int i = rowBox; i < rowBox + ROW_SIZE; i++){
        for(int j = columnBox; j < columnBox + COLUMN_SIZE; j++){
            if(s->board[i][j] == 0 && num_bits(s->possible[i][j]) == 1){
                place_number(s, first_bit(s->possible[i][j]),i,j);
            }
        }
    }

    return 1;
}

int solve_board(sudoku* sudo){
    sudoku s;
    copy_sudoku(&s, sudo);
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(s.board[i][j] == 0){
                for(int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++){
                    if(bit(s.possible[i][j],numberToTry)){
                        place_number(&s,numberToTry,i,j);
                        if(solve_board(&s)){
                            copy_sudoku(sudo, &s);
                            return 1;
                        }else {
                            copy_sudoku(&s, sudo);
                        }
                    }
                }
                return 0;
            }
        }
    }
    copy_sudoku(sudo, &s);
    return 1;
}


void output_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(i%ROW_SIZE == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j%COLUMN_SIZE == 0 && j != 0) printf("|");
            printf("%d",s[i][j]);
        }
        printf("\n");
    }
}

