#define GLSL_VERSION 330

#define DEFAULT_SCREEN_WIDTH 490
#define DEFAULT_SCREEN_HEIGHT 1173

#define DEFAULT_SHEET_HEIGHT 1100
#define TOP_BAR_HEIGHT 73

#define COLUMNS 3
#define ROWS 22
#define CELL_COUNT (COLUMNS * ROWS)
#define CELL_TEXT_LENGTH 13

#define LEVEL_COUNT 20
#define LEVELS_GE 0
#define LEVELS_PD 1

#define TEXT_VETO 0
#define TEXT_DNF  1

#define DEFAULT_FONT_SIZE 25
#define SPACING 3

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

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

#define MODE_INSERTION 0
#define MODE_DELETION 1

#define BTN_EXIT 0
#define BTN_MINIMIZE 1

#define BTN_SIZE 26

#define TEXTURE_UNHIGHLIGHTED 0
#define TEXTURE_HIGHLIGHTED 1
#define TEXTURE_SELECTED 2

#define STATE_BTN_UNHIGHLIGHTED 0
#define STATE_BTN_HIGHLIGHTED 1
#define STATE_BTN_PRESSED 2

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))