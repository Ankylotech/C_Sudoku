//
// Created by Ankylotech on 31.03.2022.
//

#include "io.h"

int readNumberLine(FILE* f){
    int num = 0;
    int c;
    while((c = fgetc(f)) < '0' || c > '9');
    do{
        num*= 10;
        num += c - '0';
    }while((c = fgetc(f)) >= '0' && c <= '9');
    return num;
}

int readSingleNumber(FILE* f){
    int num = 0;
    int c;
    while((c = fgetc(f)) < '0' || c > '9');
    num*= 10;
    num += c - '0';
    return num;
}