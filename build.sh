#!/bin/sh
set -xe
mkdir -p bin/
clang -std=c11 -Wall -pedantic -g -I./src/ ./src/main.c ./src/lexer.c ./src/arena.c ./src/data_structs.c -o ./bin/calc
