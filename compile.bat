@echo off
gcc -O0 -g3 -o build/tracker.exe src/*.c -std=c99 -Wall -Wextra -lraylib -lglfw3 -Iinclude