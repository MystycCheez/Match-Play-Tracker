#include "includes.h"

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
} MouseState;

typedef struct CollisionMap {
    bool exit;
    bool minimize;
    bool titleBar;
    bool sheet;
} CollisionMap;

typedef struct KeyMap {
    bool ctrl;
    bool shift;
    bool left;
    bool right;
    bool up;
    bool down;
    bool enter;
    bool escape;
    bool delete;
    bool backspace;
    bool c;
    bool x;
    bool v;
} KeyMap;

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
    Color color;
    Color highlight;
    Alignment alignment : 2;
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
    Texture2D texture;
    State_Button state : 2;
} Button;

typedef struct Globals {
    float screenWidth;
    float screenHeight;
    float cellWidth;
    float cellHeight;
    float fontSize;
    Specials specials;
    Button *buttons;
    Selection selection;
    Scope_Level scope;
    bool shouldExit;
} Globals;

typedef struct Node {
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

// screenWidth, screenHeight, cellWidth, cellHeight, fontSize, specials, buttons, shouldExit
Globals GVARS;