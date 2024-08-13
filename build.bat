@echo off
if not exist bin/ mkdir bin/
clang -std=c11 -Wall -pedantic -g -I./src/ ./src/main.c ./src/lexer.c ./src/arena.c ./src/data_structs.c -o ./bin/calc.exe
