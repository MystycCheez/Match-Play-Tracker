#define GLSL_VERSION 330

#define COLUMNS 3
#define ROWS 22

#define CELL_COUNT (COLUMNS * ROWS)
#define CELL_TEXT_LENGTH 17

#define BASE_CELL_HEIGHT 29
#define BASE_CELL_WIDTH 120

#define TOP_BAR_HEIGHT 50

#define SHEET_WIDTH (BASE_CELL_WIDTH * COLUMNS)
#define SHEET_HEIGHT (BASE_CELL_HEIGHT * ROWS)

#define BASE_FONT_SIZE 16
#define BASE_BUTTON_SIZE 26

#define LEVEL_COUNT 20

#define BACKGROUND_COLOR (Color){0x15, 0x17, 0x19, 0xFF}
#define BORDER_COLOR     (Color){0x66, 0x66, 0x66, 0xFF}
#define GRADIENT_TOP     (Color){0x33, 0x33, 0x33, 0xFF}
#define GRADIENT_BOTTOM  (Color){0x22, 0x22, 0x22, 0xFF}

#define COLOR_LEVEL      (Color){0xFF, 0xCC, 0x00, 0xFF}
#define TRANSPARENT      (Color){0x00, 0x00, 0x00, 0x00}

#define COLOR_WIN        (Color){0x00, 0x4B, 0x00, 0xFF}
#define COLOR_LOSE       (Color){0x4B, 0x03, 0x03, 0xFF}

#define COLOR_HIGHLIGHT  (Color){0xFF, 0xFF, 0xFF, 0x80}

#define DIR_LEFT 0
#define DIR_RIGHT 1

#define MODIFIER_KEYS 3

#define KEY_COMBO_COUNT ((KEY_COUNT * MODIFIER_KEYS) + 1)

#define UNUSED_KEY_COMBO GetKeyComboIndex(K_ESCAPE, M_CTRL_SHIFT)

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))