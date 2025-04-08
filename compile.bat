@echo off
gcc -O0 -o tracker.exe main.c -std=c99 -Wall -Wextra -Wno-unused-parameter -lraylib -lgdi32 -lwinmm