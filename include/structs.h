#ifndef STRUCTS_H
#define STRUCTS_H

// 4 components: x1, y1, x2, y2
typedef struct Line {
    float x1;
    float y1;
    float x2;
    float y2;
} Line;

// For needing pairs of Ints instead of a Vector2
// 2 components: a/b
typedef struct Int2 {
    int a;
    int b;
} Int2;

typedef struct MouseState {
    bool down;
    bool pressed;
    bool released;
    Vector2 pos;
} MouseState;

typedef struct KeyboardState {
    int pressed;
    bool shift;
    bool ctrl;
    bool alt;
} KeyboardState;

typedef struct CollisionMap {
    bool exit;
    bool minimize;
    bool titleBar;
    bool sheet;
} CollisionMap;

// 3 components: str, cStart, cEnd
typedef struct GapBuffer {
    char* str;
    size_t cStart;
    size_t cEnd;
} GapBuffer;

// start, end, exists
typedef struct Selection {
    size_t start;
    size_t end;
    bool exists;
} Selection;

// 5 components: gapStr, color, highlight, alignment, selectable
typedef struct Cell {
    GapBuffer gapStr;
    Alignment alignment : 2;
    Color color;
    Color highlight;
    bool selectable;
} Cell;

// 4 components: p1, p2, s1, s2
typedef struct Players {
    char* p1;
    char* p2;
    size_t s1;
    size_t s2;
} Players;

// List of veto/dnf variations and how many
typedef struct Specials {
    char** text;
    size_t count;
} Specials;

typedef struct Button {
    Vector2 pos;
    Vector2 size;
    Texture2D texture;
    Button_State state : 2;
} Button;

typedef struct Globals {
    Specials specials;
    Scope_Level scope;
    bool game;
    bool vetoFlag;
    bool shouldExit;
} Globals;

// Consider renaming struct
// 6 components: cellList, cell, players, selection, index, level_win
typedef struct Sheet_Info {
    Cell* cellList;
    Cell* cell;
    size_t index;
    Selection selection; // No need for more than one selection at a time
    Players players;
    char* level_win;
} Sheet_Info;

typedef struct UI_Elements {
    Line borders[COLUMNS + ROWS];
    Button *buttons;
    float cellWidth;
    float cellHeight;
    float fontSize;
    int topBarHeight;
    Font font;
    Vector2 TextPos;
} UI_Elements;

typedef struct Window_Elements {
    Vector2 scaleDPI;
    float Width;
    float Height;
    Image Icon;
    Texture IconTexture;
    bool expand;
} Window_Elements;

typedef struct TitleBar_Elements {
    Texture Texture;
    NPatchInfo NPatch;
    Rectangle Rec;
    Image Image;
} TitleBar_Elements;

typedef struct Cursor_Elements {
    MouseCursor type;
    int timer;
} Cursor_Elements;

#endif