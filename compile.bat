@echo off
gcc -O0 -o tracker.exe main.c -std=c99 -Wall -Wextra -lraylib -lgdi32 -lwinmm