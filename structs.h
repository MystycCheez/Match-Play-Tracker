#include "includes.h"

// Line, 4 components
typedef struct Line {
    float x1;
    float y1;
    float x2;
    float y2;
} Line;

typedef struct Cell {
    char* text;
    int cursor;
    unsigned char alignment : 2;
    Color color;
    Color highlight;
} Cell;

typedef struct Players {
    char* p1;
    char* p2;
    unsigned int s1;
    unsigned int s2;
} Players;