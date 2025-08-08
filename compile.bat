@echo off
gcc -O0 -g3 -o tracker.exe main.c -std=c99 -Wall -Wextra -lraylib -lglfw3