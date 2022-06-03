//
// Created by ankylotech on 29.03.22.
//
#include "sudoku.h"

void print_binary(int number) {
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        putchar(bit(number,i)+'0');
    }
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
            s1->possible[i][j] = s2->possible[i][j];
        }
    }
}

void initialize(sudoku* s, int board[SUDOKU_SIZE][SUDOKU_SIZE]){
    clear(s);
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            if(board[row][column] > 0) {
                place_number(s, board[row][column], row, column);
            }
        }
    }
}

void clear(sudoku* s){
    int val = (int)pow(2,SUDOKU_SIZE)-1;
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            s->possible[row][column] = val;
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
            tot = i;
        }
    }
    return tot;
}

int first_bit(int n){
    for(int i = 1; i <= SUDOKU_SIZE; i++){
        if(bit(n,i)) return i;
    }
    return 0;
}

int finishBoard(sudoku* s){
    int ret = 1;

    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            int num = bit_num(s->possible[i][j]);
            if(num != 1) ret = 0;
        }
    }
    return ret;
}

int naked_numbers_spec(sudoku* sudoku1, int row, int col){
    int rows = 0;
    int cols = 0;
    int boxs = 0;

    int value = sudoku1->possible[row][col];
    int count = bit_num(value);
    if(count < SUDOKU_SIZE && count > 1 ){
        int boxX = (row/SUDOKU_N)*SUDOKU_N;
        int boxY = (col/SUDOKU_N)*SUDOKU_N;

        for(int i = 0; i < SUDOKU_SIZE ; i++){
            int r = sudoku1->possible[row][i];
            int c = sudoku1->possible[i][col];

            int x = i%SUDOKU_N;
            int y = i/SUDOKU_N;

            int b = sudoku1->possible[boxX+x][boxY+y];

            if(r == value) rows++;
            if(c == value) cols++;
            if(b == value) boxs++;
        }

        if(rows < 1) return 0;
        if(cols < 1) return 0;
        if(boxs < 1) return 0;

        if(count == 1){
            rows = 1;
            cols = 1;
            boxs = 1;
        }


        for(int i = 0; i < SUDOKU_SIZE; i++) {
            int r = sudoku1->possible[row][i];
            int c = sudoku1->possible[i][col];

            int x = i%SUDOKU_N;
            int y = i/SUDOKU_N;

            int b = sudoku1->possible[boxX+x][boxY+y];

            if(rows == count && r != value){
                sudoku1->possible[row][i] &= ~value;
            }

            if(cols == count && c != value){
                sudoku1->possible[i][col] &= ~value;
            }

            if(boxs == count && b != value){
                sudoku1->possible[boxX+x][boxY+y] &= ~value;
            }

            if(sudoku1->possible[row][i] == 0 || (r != sudoku1->possible[row][i] && !naked_numbers_spec(sudoku1, row, i))) return 0;
            if(sudoku1->possible[i][col] == 0 || (c != sudoku1->possible[i][col] && !naked_numbers_spec(sudoku1, i, col))) return 0;
            if(sudoku1->possible[boxX+x][boxY+y] == 0 || (b != sudoku1->possible[boxX+x][boxY+y] && !naked_numbers_spec(sudoku1,boxX+x,boxY+y))) return 0;
        }
    }
    return 1;
}

int naked_numbers(sudoku* sudoku1){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(!naked_numbers_spec(sudoku1,i,j)){
                return 0;
            }
        }
    }
    return 1;
}


int place_number(sudoku* s, int number, int row, int column){
    s->possible[row][column] = set_bit(0,number);
    return naked_numbers_spec(s,row,column);
}

int logic(sudoku* s) {
    return naked_numbers(s);
}

int branch_bound(sudoku* sudoku1){
    //logic(sudoku1);
    int x = -1,y = -1;
    int min = SUDOKU_SIZE+1;
    int ret = 1;

    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            int num = bit_num(sudoku1->possible[i][j]);
            if(num > 1) ret = 0;
            else if(num == 0) return 0;
            if(num > 1 && num < min){
                x = i;
                y = j;
                min = num;
            }
        }
    }

    if(x != -1){
        for(int poss = 1; poss <= SUDOKU_SIZE; poss++){
            sudoku copy;
            copy_sudoku(&copy,sudoku1);
            if(bit(copy.possible[x][y],poss)){
                if(place_number(&copy,poss,x,y) && branch_bound(&copy)) {
                    copy_sudoku(sudoku1,&copy);
                    return 1;
                }
            }
        }
    }
    return ret;
}

int solve_board(sudoku* s){
    return logic(s) && branch_bound(s);
}

void output_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(i%SUDOKU_N == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j%SUDOKU_N == 0 && j != 0) printf("|");
            printf("%d", one_bit(s[i][j]));

        }
        printf("\n");
    }
}

void output_binary_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]){
    printf("\n");
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(i%SUDOKU_N == 0 && i != 0) printf("----------------------\n");
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j%SUDOKU_N == 0 && j != 0) printf("|");
            print_binary(s[i][j]);
            printf(".");
        }
        printf("\n");
    }
}

