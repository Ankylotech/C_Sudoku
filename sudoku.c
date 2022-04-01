//
// Created by ankylotech on 29.03.22.
//
#include "sudoku.h"

void print_binary(int number)
{
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
            s1->board[i][j] = s2->board[i][j];
            s1->possible[i][j] = s2->possible[i][j];
        }
    }
}

void initialize(sudoku* s, int board[SUDOKU_SIZE][SUDOKU_SIZE]){
    clear(s);
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            place_number_logicless(s,board[row][column],row,column);
        }
    }
}

void clear(sudoku* s){
    int val = pow(2,SUDOKU_SIZE)-1;;
    for(int row = 0; row < SUDOKU_SIZE; row++){
        for(int column = 0; column < SUDOKU_SIZE; column++){
            s->possible[row][column] = val;
            s->board[row][column] = 0;
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
    return tot;
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

int naked_singles(sudoku* s, int row, int column){
    int val = 1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(!s->board[row][i] && one_bit(s->possible[row][i])){
            val = -1;
            place_number(s, first_bit(s->possible[row][i]),row,i);
        }else if(s->possible[row][i] == 0) return 0;
        if(!s->board[i][column] && one_bit(s->possible[i][column])){
            val = -1;
            place_number(s, first_bit(s->possible[i][column]),i,column);
        }else if(s->possible[i][column] == 0) return 0;
    }
    int rowBox = row - row%ROW_SIZE;
    int columnBox = column - column%COLUMN_SIZE;
    for(int i = rowBox; i < rowBox + ROW_SIZE; i++){
        for(int j = columnBox; j < columnBox + COLUMN_SIZE; j++){
            if(!s->board[i][j] && one_bit(s->possible[i][j])){
                val = -1;
                place_number(s, first_bit(s->possible[i][j]),i,j);
            }else if(s->possible[i][j] == 0) return 0;
        }
    }
    return val;
}

int naked_singles_all(sudoku*s){
    int val = 1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(!s->board[i][j] && one_bit(s->possible[i][j])){
                val = -1;
                if(!place_number(s, first_bit(s->possible[i][j]),i,j)) return 0;
            }else if(s->possible[i][j] == 0) return 0;
        }
    }
    return val;
}

int hidden_singles(sudoku* sudo){
    int val = 1;
    int t = 0;
    for(int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (!one_bit(sudo->possible[i][j])) {
                for (int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++) {
                    if (bit(sudo->possible[i][j], numberToTry)) {
                        sudo->possible[i][j] = unset_bit(sudo->possible[i][j], numberToTry);
                        if (!possible(*sudo, i, j)) {
                            t = 1;
                            val &= place_number(sudo, numberToTry, i, j);
                            break;
                        }
                        sudo->possible[i][j] = set_bit(sudo->possible[i][j], numberToTry);
                    }
                }
            }
        }
    }
    if(!val && t) return -1;
    return val;
}

int naked_nums(sudoku* sudo){
    int rowtests[SUDOKU_SIZE];
    int coltests[SUDOKU_SIZE];
    int boxtests[SUDOKU_SIZE];

    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for(int j = 0; j < SUDOKU_SIZE; j++){
            rowtests[j] = 1;
            coltests[j] = 1;
            boxtests[j] = 1;
        }
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            int x1 = (i % 3) * 3 + j % 3;
            int y1 = (i / 3) * 3 + j / 3;
            for (int k = j+1; k < SUDOKU_SIZE; k++) {
                if (rowtests[j] >= 0 && sudo->possible[i][j] == sudo->possible[i][k]) {
                    rowtests[j]++;
                    rowtests[k] = -1;
                }
                if (coltests[j] >= 0 && sudo->possible[j][i] == sudo->possible[k][i]) {
                    coltests[j]++;
                    coltests[k] = -1;
                }
                int x2 = (i % 3) * 3 + k % 3;
                int y2 = (i / 3) * 3 + k / 3;
                if (boxtests[j] >= 0 && sudo->possible[x1][y1] == sudo->possible[x2][y2]) {
                    boxtests[j]++;
                    boxtests[k] = -1;
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
                    }
                }
            } else if (rowtests[l] > num) return 0;
            k = sudo->possible[l][i];
            num = bit_num(k);
            if (coltests[l] == num) {
                for (int j = 0; j < SUDOKU_SIZE; j++) {
                    if (sudo->possible[j][i] != k) {
                        sudo->possible[j][i] &= (~k);
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
                    }
                }
            } else if (boxtests[l] > num) return 0;

        }
    }
    return 1;
}

int logic(sudoku* sudo, int row, int column){
    int n = naked_singles(sudo,row,column);
    int h = hidden_singles(sudo);
    int nn = 1 ; // naked_nums(sudo);
    return h && n && nn;
}

int logic_all(sudoku* sudo){
    int h = hidden_singles(sudo);
    int n = naked_singles_all(sudo);
    int nn = 1; // naked_nums(sudo);
    return h && n && nn;
}

int logic_old(sudoku* sudo){
    return hidden_singles(sudo)
           //&& naked_nums(sudo)
            ;
}

int place_number_logicless(sudoku* s, int number, int row, int column){
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

int place_number(sudoku* s, int number, int row, int column){
    place_number_logicless(s,number,row,column);

    return logic(s, row, column);
}

int finishBoard(sudoku* s){
    int val = (int)pow(2,SUDOKU_SIZE)-1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        int rows = 0;
        int cols = 0;
        int boxes = 0;
        for(int j = 0; j < SUDOKU_SIZE; j++){

            int x = (i % 3) * 3 + j % 3;
            int y = (i / 3) * 3 + j / 3;
            if(s->board[i][j]) {
                s->possible[i][j] = set_bit(0, s->board[i][j]);
            }else if(one_bit(s->possible[i][j])){
                s->board[i][j] = first_bit(s->possible[i][j]);
            }else return 0;
            rows |= s->possible[j][i];
            cols |= s->possible[i][j];
            boxes |= s->possible[x][y];
        }
        if(rows != val || cols != val || boxes != val) return 0;
    }

    return 1;
}

int branch_bound_old(sudoku* sudo){
    sudoku s;
    copy_sudoku(&s, sudo);
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(s.board[i][j] == 0){
                for(int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++){

                    if(bit(s.possible[i][j],numberToTry)){
                        s.possible[i][j] = unset_bit(s.possible[i][j],numberToTry);
                        if(!possible(s,i,j)){
                            place_number(&s,numberToTry,i,j);
                            copy_sudoku(sudo, &s);
                            break;
                        }
                        place_number(&s,numberToTry,i,j);
                        if(solve_board_old(&s)){
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

int branch_bound(sudoku* sudo){
    //logic_all(sudo);
    sudoku s;
    copy_sudoku(&s, sudo);
    int minX = 0;
    int minY = 0;
    int num = 10;
    int full = 1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            int k = bit_num(s.possible[i][j]);
            if(k > 1 && k < num){
                minX = i;
                minY = j;
                num = k;
                full = 0;
            }
        }
    }
    if(full) return finishBoard(sudo);
    for(int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++){
        if(bit(s.possible[minX][minY],numberToTry)){
            if(place_number(&s,numberToTry,minX,minY) && branch_bound(&s)){
                copy_sudoku(sudo, &s);
                return 1;
            }else {
                copy_sudoku(&s, sudo);
            }
        }
    }

    return 0;
}

int branch_bound_pure(sudoku* sudo){
    sudoku s;
    copy_sudoku(&s, sudo);
    int minX = 0;
    int minY = 0;
    int num = 10;
    int full = 1;
    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            int k = bit_num(s.possible[i][j]);
            if(k > 1 && k < num){
                minX = i;
                minY = j;
                num = k;
                full = 0;
            }
        }
    }
    if(full) return finishBoard(sudo);
    for(int numberToTry = 1; numberToTry <= SUDOKU_SIZE; numberToTry++){
        if(bit(s.possible[minX][minY],numberToTry)){
            place_number_logicless(&s,numberToTry,minX,minY);
            if(branch_bound_pure(&s)){
                copy_sudoku(sudo, &s);
                return 1;
            }else {
                copy_sudoku(&s, sudo);
            }
        }
    }

    return 0;
}


int solve_board(sudoku* sudo){
    return logic_all(sudo) && branch_bound(sudo);
}

int solve_board_old(sudoku* sudo){
    return logic_old(sudo) && branch_bound_old(sudo);
}

int solve_board_pure(sudoku* sudo){
    return branch_bound_pure(sudo);
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

void output_binary_matrix(int s[SUDOKU_SIZE][SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(i%ROW_SIZE == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j%COLUMN_SIZE == 0 && j != 0) printf("|");
            print_binary(s[i][j]);
            printf(".");
        }
        printf("\n");
    }
}

