#include "includes.h"

// Line, 4 components
typedef struct Line {
    float x1;
    float y1;
    float x2;
    float y2;
} Line;

typedef struct Int2 {
    int x;
    int y;
} Int2;

typedef struct GapBuffer {
    char* str;
    size_t cStart;
    size_t cEnd;
} GapBuffer;

typedef struct Cell {
    GapBuffer gapStr;
    unsigned char alignment : 2;
    bool hasTime;
    Color color;
    Color highlight;
} Cell;

typedef struct Players {
    char* p1;
    char* p2;
    unsigned int s1;
    unsigned int s2;
} Players;