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
    int count[SUDOKU_SIZE];

    for(int i = 0; i < SUDOKU_SIZE; i++){
        count[i] = 0;
    }


    for(int i = 0; i < SUDOKU_SIZE; i++){
        for(int j = 0; j < SUDOKU_SIZE; j++){
            int num = bit_num(s->possible[i][j]);
            if(num != 1) ret = 0;
            count[first_bit(s->possible[i][j])-1]++;
        }
    }
    for(int i = 0; i < SUDOKU_SIZE; i++){
        printf("%d\n",count[i]);
        if(count[i] != SUDOKU_SIZE) return 0;
    }
    return ret;
}

int contains(int a, int b){
    return (a & b) == b;
}

void count_occurrence(sudoku* sudoku1, int row, int col, int value, int* row_count, int* col_count, int* box_count){
    int boxX = (row/SUDOKU_N)*SUDOKU_N;
    int boxY = (col/SUDOKU_N)*SUDOKU_N;

    for(int i = 0; i < SUDOKU_SIZE ; i++){
        int r = sudoku1->possible[row][i];
        int c = sudoku1->possible[i][col];

        int x = i%SUDOKU_N;
        int y = i/SUDOKU_N;

        int b = sudoku1->possible[boxX+x][boxY+y];

        if(r == value) (*row_count)++;
        if(c == value) (*col_count)++;
        if(b == value) (*box_count)++;
    }
}

void count_containing(sudoku* sudoku1, int row, int col, int value, int* row_count, int* col_count, int* box_count){
    int boxX = (row/SUDOKU_N)*SUDOKU_N;
    int boxY = (col/SUDOKU_N)*SUDOKU_N;

    for(int i = 0; i < SUDOKU_SIZE ; i++){
        int r = sudoku1->possible[row][i];
        int c = sudoku1->possible[i][col];

        int x = i%SUDOKU_N;
        int y = i/SUDOKU_N;

        int b = sudoku1->possible[boxX+x][boxY+y];

        if(contains(r, value)) (*row_count)++;
        if(contains(c, value)) (*col_count)++;
        if(contains(b, value)) (*box_count)++;
    }
}

int naked_numbers_spec(sudoku* sudoku1, int row, int col){
    int rowOccurrences = 0;
    int colOccurrences = 0;
    int boxOccurrences = 0;

    int rowChanges = 0;
    int colChanges = 0;
    int boxChanges = 0;

    int value = sudoku1->possible[row][col];
    int count = bit_num(value);

    if(count < SUDOKU_SIZE && count > 0 ){
        int boxX = (row/SUDOKU_N)*SUDOKU_N;
        int boxY = (col/SUDOKU_N)*SUDOKU_N;

        count_occurrence(sudoku1, row, col, value, &rowOccurrences, &colOccurrences, &boxOccurrences);

        if(rowOccurrences < 1 || rowOccurrences > count) return 0;
        if(colOccurrences < 1 || colOccurrences > count) return 0;
        if(boxOccurrences < 1 || boxOccurrences > count) return 0;

        for(int i = 0; i < SUDOKU_SIZE; i++) {
            int rowValue = sudoku1->possible[row][i];
            int colValue = sudoku1->possible[i][col];

            int x = boxX + i%SUDOKU_N;
            int y = boxY + i/SUDOKU_N;

            int boxValue = sudoku1->possible[x][y];

            if(rowOccurrences == count && rowValue != value){
                sudoku1->possible[row][i] &= ~value;
            }

            if(colOccurrences == count && colValue != value){
                sudoku1->possible[i][col] &= ~value;
            }

            if(boxOccurrences == count && boxValue != value){
                sudoku1->possible[x][y] &= ~value;
            }

            if(rowValue != sudoku1->possible[row][i]) rowChanges = set_bit(rowChanges,i+1);
            if(colValue != sudoku1->possible[i][col]) colChanges = set_bit(colChanges,i+1);
            if(boxValue != sudoku1->possible[x][y])   boxChanges = set_bit(boxChanges,i+1);
        }

        for(int i = 0; i < SUDOKU_SIZE; i++) {
            int x = boxX + i%SUDOKU_N;
            int y = boxY + i/SUDOKU_N;
            if(bit(rowChanges,i+1) && (sudoku1->possible[row][i] == 0 || !logic_spec(sudoku1,   row,i))) return 0;
            if(bit(colChanges,i+1) && (sudoku1->possible[i][col] == 0 || !logic_spec(sudoku1,i,  col))) return 0;
            if(bit(boxChanges,i+1) && (sudoku1->possible[x][y]   == 0 || !logic_spec(sudoku1,x,y))) return 0;
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

int hidden_numbers_spec(sudoku* sudoku1, int row, int col){
    int value = sudoku1->possible[row][col];

    for(int b = 1; b <= SUDOKU_SIZE; b++){
        int rowOccurrences = 0;
        int colOccurrences = 0;
        int boxOccurrences = 0;
        int currentOne = set_bit(0, b);

        if(bit(value,b) && currentOne != value){

            count_containing(sudoku1, row, col, currentOne, &rowOccurrences, &colOccurrences, &boxOccurrences);

            if(rowOccurrences < 1) return 0;
            if(colOccurrences < 1) return 0;
            if(boxOccurrences < 1) return 0;

            if(rowOccurrences == 1 || colOccurrences == 1 || boxOccurrences == 1){
                sudoku1->possible[row][col] = currentOne;
                return logic_spec(sudoku1,row,col);
            }
        }
    }
    return 1;
}


int place_number(sudoku* s, int number, int row, int column){
    s->possible[row][column] = set_bit(0,number);
    return logic_spec(s,row,column);
}

int logic_spec(sudoku* sudoku1, int row, int col){
    int hidden = 1;
    int naked = 1;
    hidden = hidden_numbers_spec(sudoku1,row,col);
    naked = naked_numbers_spec(sudoku1,row,col);
    return  hidden && naked;
}

int logic(sudoku* s) {
    return naked_numbers(s);
}

int branch_bound(sudoku* sudoku1){
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
        sudoku copy;
        for(int poss = 1; poss <= SUDOKU_SIZE; poss++){
            copy_sudoku(&copy,sudoku1);
            if(bit(copy.possible[x][y],poss)){
                if(place_number(&copy,poss,x,y) != 0 && branch_bound(&copy)) {
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

void output_board(int s[SUDOKU_SIZE][SUDOKU_SIZE]){
    for(int i = 0; i < SUDOKU_SIZE; i++){
        if(i%SUDOKU_N == 0 && i != 0) printf("----------\n");
        for(int j = 0; j < SUDOKU_SIZE; j++){
            if(j%SUDOKU_N == 0 && j != 0) printf("|");
            printf("%d", s[i][j]);

        }
        printf("\n");
    }
}

