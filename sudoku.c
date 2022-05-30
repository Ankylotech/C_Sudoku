//
// Created by ankylotech on 29.03.22.
//
#include "sudoku.h"

void print_binary(int number) {
    for(int i = 1; i <= SUDOKU_SIZE*SUDOKU_SIZE; i++){
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
    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){
            s1->possible[i][j] = s2->possible[i][j];
        }
    }
}

void initialize(sudoku* s, int board[SUDOKU_SIZE*SUDOKU_SIZE][SUDOKU_SIZE*SUDOKU_SIZE]){
    clear(s);
    for(int row = 0; row < SUDOKU_SIZE*SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE*SUDOKU_SIZE; column++){
            if(board[row][column] > 0)place_number(s,board[row][column],row,column);
        }
    }
}

void clear(sudoku* s){
    int val = pow(2,SUDOKU_SIZE*SUDOKU_SIZE)-1;;
    for(int row = 0; row < SUDOKU_SIZE*SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE*SUDOKU_SIZE; column++){
            s->possible[row][column] = val;
        }
    }
}

int bit_num(int n){
    int tot = 0;
    for(int i = 1; i <= SUDOKU_SIZE*SUDOKU_SIZE; i++){
        if(bit(n,i)) tot++;
    }
    return tot;
}


int one_bit(int n){
    int tot = 0;
    for(int i = 1; i <= SUDOKU_SIZE*SUDOKU_SIZE; i++){
        if(bit(n,i)) {
            if(tot) return 0;
            tot++;
        }
    }
    return tot;
}

int first_bit(int n){
    for(int i = 1; i <= SUDOKU_SIZE*SUDOKU_SIZE; i++){
        if(bit(n,i)) return i;
    }
    return 0;
}

int finishBoard(sudoku* s){
    int ret = 1;

    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){
            int num = bit_num(s->possible[i][j]);
            if(num != 1) ret = 0;
        }
    }
    return ret;
}

void naked_numbers(sudoku* sudoku1){
    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        int rows[SUDOKU_SIZE*SUDOKU_SIZE];
        int cols[SUDOKU_SIZE*SUDOKU_SIZE];
        int boxs[SUDOKU_SIZE*SUDOKU_SIZE];

        // count the appearances of the possibilities in rows,columns and boxes

        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){

            int row = sudoku1->possible[i][j];
            int col = sudoku1->possible[j][i];

            int bx = (i%3)*3 + j%3;
            int by = (i/3)*3 + j/3;

            int box = sudoku1->possible[bx][by];

            int reset = 0;

            rows[j] = 1;
            cols[j] = 1;
            boxs[j] = 1;

            for(int k = j+1; k < SUDOKU_SIZE*SUDOKU_SIZE; k++){

                int nrow = sudoku1->possible[i][k];
                int ncol = sudoku1->possible[k][i];

                int bx2 = (i%3)*3 + k%3;
                int by2 = (i/3)*3 + k/3;

                int nbox = sudoku1->possible[bx2][by2];

                if(row == nrow){
                    rows[j]++;
                }
                if(col == ncol){
                    cols[j]++;
                }
                if(box == nbox){
                    boxs[j]++;
                }
            }

            if(rows[j] == bit_num(row) && bit_num(row) < SUDOKU_SIZE*SUDOKU_SIZE){
                for(int k = 0; k < SUDOKU_SIZE*SUDOKU_SIZE; k++){
                    int num = sudoku1->possible[i][k];
                    if(num != row){
                        sudoku1->possible[i][k] &= ~row;
                    }
                    if(num != sudoku1->possible[i][k]) {
                        reset = 1;
                    }
                }
            }

            if(cols[j] == bit_num(col) && bit_num(col) < SUDOKU_SIZE*SUDOKU_SIZE){
                for(int k = 0; k < SUDOKU_SIZE*SUDOKU_SIZE; k++){
                    int num = sudoku1->possible[k][i];
                    if(num != col){
                        sudoku1->possible[k][i] &= ~col;
                    }
                    if(num != sudoku1->possible[k][i]) {
                        reset = 1;
                    }
                }
            }

            if(boxs[j] == bit_num(box) && bit_num(box) < SUDOKU_SIZE*SUDOKU_SIZE){
                for(int k = 0; k < SUDOKU_SIZE*SUDOKU_SIZE; k++){
                    int bx2 = (i%3)*3 + k%3;
                    int by2 = (i/3)*3 + k/3;
                    int num = sudoku1->possible[bx2][by2];
                    if(num != box){
                        sudoku1->possible[bx2][by2] &= ~box;
                    }
                    if(num != sudoku1->possible[bx2][by2]) {
                        reset = 1;
                    }
                }
            }

            if(reset) {
                i = -1;
                j = SUDOKU_SIZE*SUDOKU_SIZE;
            }
        }
    }
}


int place_number(sudoku* s, int number, int row, int column){
    s->possible[row][column] = set_bit(0,number);
}

int logic(sudoku* s){
    naked_numbers(s);
    return 1;
}

int branch_bound(sudoku* sudoku1){
    logic(sudoku1);
    sudoku copy;
    copy_sudoku(&copy,sudoku1);
    int x = -1,y = -1;
    int min = SUDOKU_SIZE*SUDOKU_SIZE+1;
    int number;
    int ret = 1;

    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){
            int num = bit_num(sudoku1->possible[i][j]);
            if(num > 1) ret = 0;
            else if(num == 0) return 0;
            if(num > 1 && num < min){
                x = i;
                y = j;
                min = num;
                number = sudoku1->possible[i][j];
            }
        }
    }

    if(x != -1){
        printf("iterating\n");
        for(int poss = 1; poss <= SUDOKU_SIZE*SUDOKU_SIZE; poss++){
            if(bit(copy.possible[x][y],poss)){
                printf("placing %d at %d,%d\n",poss,x,y);
                place_number(&copy,poss,x,y);
                if(branch_bound(&copy)) {
                    copy_sudoku(sudoku1,&copy);
                    return 1;
                }
                else {
                    copy.possible[x][y] = number;
                }
            }
        }
    }
    return ret;
}

int solve_board(sudoku* s){
    return branch_bound(s);
}

void output_matrix(int s[SUDOKU_SIZE*SUDOKU_SIZE][SUDOKU_SIZE*SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        if(i%SUDOKU_SIZE == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){
            if(j%SUDOKU_SIZE == 0 && j != 0) printf("|");
            if(bit_num(s[i][j]) == 1) printf("%d", first_bit(s[i][j]));
            else printf("0");
        }
        printf("\n");
    }
}

void output_binary_matrix(int s[SUDOKU_SIZE*SUDOKU_SIZE][SUDOKU_SIZE*SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE*SUDOKU_SIZE; i++){
        if(i%SUDOKU_SIZE == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE*SUDOKU_SIZE; j++){
            if(j%SUDOKU_SIZE == 0 && j != 0) printf("|");
            print_binary(s[i][j]);
            printf(".");
        }
        printf("\n");
    }
}

