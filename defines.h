#define GLSL_VERSION 330

#define TOP_BAR_HEIGHT 50
#define DEFAULT_SHEET_HEIGHT 1100

#define DEFAULT_SCREEN_WIDTH 490
#define DEFAULT_SCREEN_HEIGHT DEFAULT_SHEET_HEIGHT + TOP_BAR_HEIGHT

#define COLUMNS 3
#define ROWS 22
#define CELL_COUNT (COLUMNS * ROWS)
#define CELL_TEXT_LENGTH 13

#define DEFAULT_FONT_SIZE 25
#define SPACING 3

#define BUTTON_SIZE 26

#define LEVEL_COUNT 20

#define DEFAULT_CELL_HEIGHT (DEFAULT_SHEET_HEIGHT / ROWS)
#define DEFAULT_CELL_WIDTH  (DEFAULT_SCREEN_WIDTH / COLUMNS)

#define BACKGROUND_COLOR (Color){0x15, 0x17, 0x19, 0xFF}
#define BORDER_COLOR     (Color){0x66, 0x66, 0x66, 0xFF}
#define GRADIENT_TOP     (Color){0x33, 0x33, 0x33, 0xFF}
#define GRADIENT_BOTTOM  (Color){0x22, 0x22, 0x22, 0xFF}

#define COLOR_LEVEL      (Color){0xFF, 0xCC, 0x00, 0xFF}
#define TRANSPARENT      (Color){0x00, 0x00, 0x00, 0x00}

#define COLOR_WIN        (Color){0x00, 0x4B, 0x00, 0xFF}
#define COLOR_LOSE       (Color){0x4B, 0x03, 0x03, 0xFF}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))