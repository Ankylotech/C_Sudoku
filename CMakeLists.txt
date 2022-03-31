cmake_minimum_required(VERSION 3.21)
project(C_Sudoku C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_Sudoku
        main.c
        sudoku.c sudoku.h io.c io.h)
