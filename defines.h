#define GLSL_VERSION 330

#define SCREEN_WIDTH 490
#define SCREEN_HEIGHT 1100

#define COLUMNS 3
#define ROWS 22
#define CELL_COUNT (COLUMNS * ROWS)
#define CELL_TEXT_LENGTH 11

#define LEVEL_COUNT 20

#define FONT_SIZE 25
#define SPACING 3

#define DEFAULT_CELL_HEIGHT (GetScreenHeight() / ROWS)
#define DEFAULT_CELL_WIDTH  (GetScreenWidth() / COLUMNS)

#define SELECTION_AREA (Rectangle){\
    (float)DEFAULT_CELL_WIDTH, 0,\
    (float)DEFAULT_CELL_WIDTH * 2,\
    (DEFAULT_CELL_HEIGHT * 21)}

#define BACKGROUND_COLOR (Color){0x15, 0x17, 0x19, 0xFF}
#define BORDER_COLOR     (Color){0x66, 0x66, 0x66, 0xFF}
#define GRADIENT_TOP     (Color){0x33, 0x33, 0x33, 0xFF}
#define GRADIENT_BOTTOM  (Color){0x22, 0x22, 0x22, 0xFF}

#define COLOR_LEVEL      (Color){0xFF, 0xCC, 0x00, 0xFF}
#define TRANSPARENT      (Color){0x00, 0x00, 0x00, 0x00}

#define COLOR_WIN        (Color){0x00, 0x4B, 0x00, 0xFF}
#define COLOR_LOSE       (Color){0x4B, 0x03, 0x03, 0xFF}

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

#define MODE_INSERTION 0
#define MODE_DELETION 1

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))