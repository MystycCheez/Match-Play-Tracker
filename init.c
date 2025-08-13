#ifndef INIT_C
#define INIT_C

#include "includes.h"

void initGlobals()
{
    GVARS.scaleDPI = GetWindowScaleDPI();

    UI.cellWidth = BASE_CELL_WIDTH * GVARS.scaleDPI.x;
    UI.cellHeight = BASE_CELL_HEIGHT * GVARS.scaleDPI.y;
    UI.fontSize = BASE_FONT_SIZE * GVARS.scaleDPI.x;

    Cursor.type = MOUSE_CURSOR_DEFAULT;

    GVARS.players = (Players){"Player 1", "Player 2", 0, 0};
    GVARS.game = LEVELS_GE;
    GVARS.selectedCellIndex = 0;
    GVARS.scope = SCOPE_SHEET;
    GVARS.shouldExit = false;
    GVARS.level_win = NULL;
}

void initWindow()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0, 0, "Match Play Tracker");
    Window.Icon = LoadImage("resources/logo-transparent.png");
    Window.IconTexture = LoadTextureFromImage(Window.Icon);
    SetTextureFilter(Window.IconTexture, TEXTURE_FILTER_BILINEAR);
    SetWindowIcon(Window.Icon);

    GVARS.scaleDPI = GetWindowScaleDPI();

    Window.Width = SHEET_WIDTH * GVARS.scaleDPI.x;
    Window.Height = (SHEET_HEIGHT * GVARS.scaleDPI.y) + TOP_BAR_HEIGHT;

    int currentMonitor = GetCurrentMonitor();
    int monitorCount = 0;
    int monitorWorkareaX = 0;
    int monitorWorkareaY = 0;
    int monitorWorkareaWidth = 0;
    int monitorWorkareaHeight = 0;

    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    glfwGetMonitorWorkarea(
        monitors[currentMonitor], 
        &monitorWorkareaX, 
        &monitorWorkareaY,
        &monitorWorkareaWidth,
        &monitorWorkareaHeight
    );

    SetWindowSize(Window.Width, Window.Height);
    SetWindowPosition(
        (monitorWorkareaWidth / 2) - (Window.Width / 2), 
        (monitorWorkareaHeight / 2) - (Window.Height / 2)
    );
    
    TitleBar.Image = LoadImage("resources/title-bar.png");
    TitleBar.Texture = LoadTextureFromImage(TitleBar.Image);
    TitleBar.NPatch = (NPatchInfo){(Rectangle){0, 0, 1, TOP_BAR_HEIGHT}, 0, 0, 0, 0, NPATCH_THREE_PATCH_HORIZONTAL};
    TitleBar.Rec = (Rectangle){0, 0, Window.Width, TOP_BAR_HEIGHT};
}

void initButtons()
{
    UI.buttons = malloc(sizeof(Button) * 2);
    for (size_t i = 0; i < 2; i++) {
        UI.buttons[i].size.x = BASE_BUTTON_SIZE;
        UI.buttons[i].size.y = BASE_BUTTON_SIZE;
    }
    UI.buttons[BTN_EXIT].pos = (Vector2){Window.Width - BASE_BUTTON_SIZE - (BASE_BUTTON_SIZE / 2), (TOP_BAR_HEIGHT / 2) - 13};
    UI.buttons[BTN_EXIT].texture = LoadTextureFromImage(LoadImage("resources/x.png"));
    UI.buttons[BTN_EXIT].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_EXIT].texture, TEXTURE_FILTER_BILINEAR);
    UI.buttons[BTN_MINIMIZE].pos = (Vector2){Window.Width - (BASE_BUTTON_SIZE * 3), (TOP_BAR_HEIGHT / 2) - 13};
    UI.buttons[BTN_MINIMIZE].texture = LoadTextureFromImage(LoadImage("resources/minimize.png"));
    UI.buttons[BTN_MINIMIZE].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_MINIMIZE].texture, TEXTURE_FILTER_BILINEAR);
}

void initSheetText(Cell *sheet)
{
    char **levelText = loadLevelText(GVARS.game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&sheet[(i * 3) + 3].gapStr, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%lld", GVARS.players.s1);
    sprintf(s2, "%lld", GVARS.players.s2);
    placeString(&sheet[0].gapStr, "Stage", CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 3].gapStr, "Points", CELL_TEXT_LENGTH);
    placeString(&sheet[1].gapStr, GVARS.players.p1, CELL_TEXT_LENGTH);
    placeString(&sheet[2].gapStr, GVARS.players.p2, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 2].gapStr, s1, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 1].gapStr, s2, CELL_TEXT_LENGTH);
}

Cell *initSheet()
{
    Cell *sheet = malloc(sizeof(Cell) * CELL_COUNT);
    for (size_t i = 0; i < CELL_COUNT; i++) {
        sheet[i].gapStr = initGapStr(CELL_TEXT_LENGTH);
        sheet[i].alignment = ALIGN_CENTER;
        sheet[i].color = WHITE;
        sheet[i].highlight = TRANSPARENT;
        sheet[i].selectable = true;
    }
    sheet[0].selectable = false;
    sheet[CELL_COUNT - 1].selectable = false;
    sheet[CELL_COUNT - 2].selectable = false;
    sheet[CELL_COUNT - 3].selectable = false;
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        sheet[3 + (i * COLUMNS)].alignment = ALIGN_LEFT;
        sheet[3 + (i * COLUMNS)].color = COLOR_LEVEL;
        sheet[3 + (i * COLUMNS)].selectable = false;
    }
    initSheetText(sheet);
    return sheet;
}

void setBorderPositions()
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        UI.borders[i] = (Line){0, (UI.cellHeight * i) + TOP_BAR_HEIGHT, Window.Width, (UI.cellHeight * i) + TOP_BAR_HEIGHT};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        UI.borders[i + index] = (Line){UI.cellWidth * i, TOP_BAR_HEIGHT, UI.cellWidth * i, Window.Height};
    }
}

void initActionTable()
{
    #define ACTION_DEBUG
    #ifdef ACTION_DEBUG
        char* filename = "action_table.txt";

        FILE* file_ptr = fopen(filename, "w");
        if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
        }
    #endif
    // Just init everything to do nothing for now
    for (size_t y = 0; y < KEY_COUNT * 4; y++) {
        for (size_t x = 0; x < 3; x++) {
            ActionTable[x][y] = A_DONOTHING;
        }
    }
    #ifdef ACTION_DEBUG
        fprintf(file_ptr, "                                  |                 OVERVIEW |                    SHEET |                     CELL |\n");
        for (size_t y = 0; y < KEY_COUNT * 4; y++) {
            fprintf(file_ptr, " %32s ", GetKeyComboText(y));
            for (size_t x = 0; x < 3; x++) {
                fprintf(file_ptr, "| %24d ", ActionTable[x][y]);
            }
            fprintf(file_ptr, "|\n");
        }

        fclose(file_ptr);
    #endif

    exit(1);
}

#endif