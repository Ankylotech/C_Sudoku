//
// Created by ankylotech on 29.03.22.
//
#include <stdlib.h>
#include "sudoku.h"

void print_binary(unsigned int number)
{
    if (number >> 1) {
        print_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

int set_bit(int num, int bit){
    return (num | (1<<(bit-1)));
}

int unset_bit(int num, int bit){
    return (num & (~(1<<(bit-1))));
}

int bit(int num, int bit){
    return (num >> (bit-1)) & 1;
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
    int val = pow(2,SUDOKU_SIZE)-1;;
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            s->possible[row][column] = val;
            s->board[row][column] = 0;
        }
    }
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            place_number(s,board[row][column],row,column);
        }
    }
}

int bit_num(int n){
    int tot = 0;
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) tot++;
    }
    return tot;
}


int one_bit(int n){
    int tot = 0;
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) {
            if(tot) return 0;
            tot++;
        }
    }
    return 1;
}

int first_bit(int n){
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) return i;
    }
    return 0;
}

int possible(sudoku s, int row, int column){
    int val = (int)pow(2,SUDOKU_SIZE)-1;
    int rows = 0;
    int cols = 0;
    int boxes = 0;
    for(int j = 0; j < SUDOKU_SIZE; j++) {
        rows |= s.possible[row][j];
        cols |= s.possible[j][column];
    }
    int rowBox = row - row%ROW_SIZE;
    int columnBox = column - column%COLUMN_SIZE;
    for(int i = rowBox; i < rowBox + ROW_SIZE; i++){
        for(int j = columnBox; j < columnBox + COLUMN_SIZE; j++){
            boxes |= s.possible[i][j];
        }
    }
    if(rows != val || cols != val || boxes != val) {
        return 0;
    }
    return 1;
}

void naked_singles(sudoku* s, int row, int column){
    int rowBox = row - row%ROW_SIZE;
    int columnBox = column - column%COLUMN_SIZE;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(s->board[i][column] == 0 && one_bit(s->possible[i][column]) == 1){
            place_number(s, first_bit(s->possible[i][column]),i,column);
        }
        if(s->board[row][i] == 0 && one_bit(s->possible[row][i]) == 1){
            place_number(s, first_bit(s->possible[row][i]),row,i);
        }
    }
    for(int i = rowBox; i < rowBox + ROW_SIZE; i++){
        for(int j = columnBox; j < columnBox + COLUMN_SIZE; j++){
            if(s->board[i][j] == 0 && one_bit(s->possible[i][j])){
                place_number(s, first_bit(s->possible[i][j]),i,j);
            }
        }
    }
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

    s->possible[row][column] = set_bit(0,number);

    naked_singles(s,row,column);

    return 1;
}



int branch_bound(sudoku* sudo){
    sudoku s;
    copy_sudoku(&s, sudo);
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(s.board[i][j] == 0){
                for(int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++){
                    if(bit(s.possible[i][j],numberToTry)){
                        s.possible[i][j] = unset_bit(s.possible[i][j],numberToTry);
                        if(!possible(s,i,j) || s.possible[i][j] == 0) {
                            place_number(&s,numberToTry,i,j);
                            break;
                        }
                        place_number(&s,numberToTry,i,j);
                        if(solve_board(&s)){
                            copy_sudoku(sudo, &s);
                            return 1;
                        }else {
                            copy_sudoku(&s, sudo);
                        }
                    }
                    if(numberToTry == SUDOKU_SIZE) return 0;
                }
            }
        }
    }
    return 1;
}

int naked_nums(sudoku* sudo){
    char* rowtests = calloc(SUDOKU_SIZE,1);
    char* coltests = calloc(SUDOKU_SIZE,1);
    char* boxtests = calloc(SUDOKU_SIZE,1);

    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            int x1 = (i % 3) * 3 + j % 3;
            int y1 = (i / 3) * 3 + j / 3;
            for (int k = j; k < SUDOKU_SIZE; k++) {
                if (sudo->possible[i][j] == sudo->possible[i][k]) {
                    rowtests[j]++;
                    if (k != j)rowtests[k]++;
                }
                if (sudo->possible[j][i] == sudo->possible[k][i]) {
                    coltests[j]++;
                    if (k != j)coltests[k]++;
                }
                int x2 = (i % 3) * 3 + k % 3;
                int y2 = (i / 3) * 3 + k / 3;
                if (sudo->possible[x1][y1] == sudo->possible[x2][y2]) {
                    boxtests[j]++;
                    if (k != j)boxtests[k]++;
                }
            }
        }

        for (int l = 0; l < SUDOKU_SIZE; l++) {
            int k = sudo->possible[i][l];
            int num = bit_num(k);
            if (rowtests[l] == num) {
                for (int j = 0; j < SUDOKU_SIZE; j++) {
                    if (sudo->possible[i][j] != k) {
                        sudo->possible[i][j] &= (~k);
                    } else {
                        rowtests[j] = 0;
                    }
                }
            } else if (rowtests[l] > num) return 0;
            k = sudo->possible[l][i];
            num = bit_num(k);
            if (coltests[l] == num) {
                for (int j = 0; j < SUDOKU_SIZE; j++) {
                    if (sudo->possible[j][i] != k) {
                        sudo->possible[j][i] &= (~k);
                    } else {
                        coltests[j] = 0;
                    }
                }
            } else if (coltests[l] > num) return 0;
            int x1 = (i % 3) * 3 + l % 3;
            int y1 = (i / 3) * 3 + l / 3;
            k = sudo->possible[x1][y1];
            num = bit_num(k);
            if (boxtests[l] == num) {
                for (int j = 0; j < SUDOKU_SIZE; j++) {
                    int x2 = (i % 3) * 3 + j % 3;
                    int y2 = (i / 3) * 3 + j / 3;
                    if (sudo->possible[x2][y2] != k) {
                        sudo->possible[x2][y2] &= (~k);
                    } else {
                        boxtests[j] = 0;
                    }
                }
            } else if (boxtests[l] > num) return 0;
            rowtests[l] = 0;
            coltests[l] = 0;
            boxtests[l] = 0;
        }
    }
    free(rowtests);
    free(coltests);
    free(boxtests);
    return 1;
}

int hidden_singles(sudoku* sudo){
    for(int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (sudo->board[i][j] == 0) {
                for (int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++) {
                    if (bit(sudo->possible[i][j], numberToTry)) {
                        sudo->possible[i][j] = unset_bit(sudo->possible[i][j], numberToTry);
                        if (!possible(*sudo, i, j) || sudo->possible[i][j] == 0) {
                            place_number(sudo, numberToTry, i, j);
                            break;
                        }
                        sudo->possible[i][j] = set_bit(sudo->possible[i][j], numberToTry);
                    }
                }
            }
        }
    }
    return 1;
}

int logic(sudoku* sudo){
    return naked_nums(sudo) && hidden_singles(sudo);
}

int solve_board(sudoku* sudo){
    return logic(sudo) && branch_bound(sudo);
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

