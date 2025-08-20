#ifndef INIT_C
#define INIT_C

#include "headers.h"

void initGlobals()
{
    GVARS.scaleDPI = GetWindowScaleDPI();

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

    UI.cellWidth = BASE_CELL_WIDTH * GVARS.scaleDPI.x;
    UI.cellHeight = BASE_CELL_HEIGHT * GVARS.scaleDPI.y;
    UI.fontSize = BASE_FONT_SIZE * GVARS.scaleDPI.x;
    UI.topBarHeight = TOP_BAR_HEIGHT;

    Window.Width = SHEET_WIDTH * GVARS.scaleDPI.x;
    Window.Height = (SHEET_HEIGHT * GVARS.scaleDPI.y) + UI.topBarHeight;

    Window.expand = false;

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
    TitleBar.NPatch = (NPatchInfo){(Rectangle){0, 0, 1, UI.topBarHeight}, 0, 0, 0, 0, NPATCH_THREE_PATCH_HORIZONTAL};
    TitleBar.Rec = (Rectangle){0, 0, Window.Width, UI.topBarHeight};
}

void initButtons()
{
    UI.buttons = malloc(sizeof(Button) * 2);
    for (size_t i = 0; i < 2; i++) {
        UI.buttons[i].size.x = BASE_BUTTON_SIZE;
        UI.buttons[i].size.y = BASE_BUTTON_SIZE;
    }
    UI.buttons[BTN_EXIT].pos = (Vector2){Window.Width - BASE_BUTTON_SIZE - (BASE_BUTTON_SIZE / 2), (UI.topBarHeight / 2) - 13};
    UI.buttons[BTN_EXIT].texture = LoadTextureFromImage(LoadImage("resources/x.png"));
    UI.buttons[BTN_EXIT].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_EXIT].texture, TEXTURE_FILTER_BILINEAR);
    UI.buttons[BTN_MINIMIZE].pos = (Vector2){Window.Width - (BASE_BUTTON_SIZE * 3), (UI.topBarHeight / 2) - 13};
    UI.buttons[BTN_MINIMIZE].texture = LoadTextureFromImage(LoadImage("resources/minimize.png"));
    UI.buttons[BTN_MINIMIZE].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_MINIMIZE].texture, TEXTURE_FILTER_BILINEAR);
}

void setGameText()
{
    char** levelText = loadLevelText(GVARS.game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        OverwriteStr(&sheet[(i * 3) + 3].gapStr, levelText[i], 0, CELL_TEXT_LENGTH);
    }
    free(levelText);
}

void initSheetText()
{
    setGameText();
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

void initSheet()
{
    sheet = malloc(sizeof(Cell) * CELL_COUNT);
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
}

void setBorderPositions()
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        UI.borders[i] = (Line){0, (UI.cellHeight * i) + UI.topBarHeight, Window.Width, (UI.cellHeight * i) + UI.topBarHeight};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        UI.borders[i + index] = (Line){UI.cellWidth * i, UI.topBarHeight, UI.cellWidth * i, Window.Height};
    }
}

void initActionTable()
{
    for (size_t x = 0; x < 3; x++) {
        for (size_t y = 0; y < KEY_COMBO_COUNT; y++) {
            ActionTable[x][y] = A_DONOTHING;
        }
    }

    // For all scopes
    for (size_t scope = 0; scope < 3; scope++) {
        ActionTable[scope][GetKeyComboIndex(K_S, M_CTRL)] = A_SAVETIMES;
        ActionTable[scope][GetKeyComboIndex(K_L, M_CTRL)] = A_LOADTIMES;
        ActionTable[scope][GetKeyComboIndex(K_B, M_CTRL)] = A_EXPORTTIMES;
        ActionTable[scope][GetKeyComboIndex(K_DELETE, M_CTRL)] = A_CLEARTIMES;
        ActionTable[scope][GetKeyComboIndex(K_D, M_CTRL)] = A_CLEARTIMES;
        ActionTable[scope][GetKeyComboIndex(K_ESCAPE, M_NONE)] = A_SCOPEDECREASE;
        ActionTable[scope][GetKeyComboIndex(K_C, M_ALT)] = A_SWAPVETOCOLOR;
    }

    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_ENTER, M_NONE)] = A_NAVIGATETONEXTCELL;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_ENTER, M_NONE)] = A_OVERWRITE_UPDATESCORE;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_ESCAPE, M_NONE)] = A_DESELECT_OR_UNDO_BACKOUT;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_LEFT, M_SHIFT)] = A_SELECTCHAR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_RIGHT, M_SHIFT)] = A_SELECTCHAR;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_LEFT, M_CTRL_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_RIGHT, M_CTRL_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_A, M_CTRL)] = A_SELECTALL;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_LEFT, M_NONE)] = A_MOVECURSOR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_RIGHT, M_NONE)] = A_MOVECURSOR;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_LEFT, M_CTRL)] = A_MOVECURSORTOSTART;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_RIGHT, M_CTRL)] = A_MOVECURSORTOEND;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_HOME, M_NONE)] = A_MOVECURSORTOSTART;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_END, M_NONE)] = A_MOVECURSORTOEND;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_HOME, M_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_END, M_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_HOME, M_CTRL_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR; // is this needed?
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_END, M_CTRL_SHIFT)] = A_SELECTALLATCURSORTOWARDSDIR;  // is this needed?

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_C, M_CTRL)] = A_COPY;
    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_C, M_CTRL)] = A_COPY_ALL;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_X, M_CTRL)] = A_CUT;
    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_X, M_CTRL)] = A_CUT_ALL;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_V, M_CTRL)] = A_PASTE;
    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_V, M_CTRL)] = A_PASTE_INTO_CELL;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_BACKSPACE, M_NONE)] = A_DELETE;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_DELETE, M_NONE)] = A_DELETESELECTION;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_DELETE, M_SHIFT)] = A_DELETECELLTEXT;

    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_DELETE, M_NONE)] = A_DELETECELLTEXT;
    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_BACKSPACE, M_NONE)] = A_DELETECELLTEXTANDENTERINTO;

    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_TAB, M_NONE)] = A_NAVIGATETONEXTCELL;
    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_TAB, M_NONE)] = A_OVERWRITE_UPDATESCORE;

    ActionTable[SCOPE_CELL][GetKeyComboIndex(K_R, M_CTRL)] = A_RESET_TEXT_COLOR;
    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_R, M_CTRL)] = A_RESET_TEXT_COLOR;

    ActionTable[SCOPE_SHEET][GetKeyComboIndex(K_TAB, M_CTRL)] = A_SWAPGAME;
    ActionTable[SCOPE_OVERVIEW][GetKeyComboIndex(K_TAB, M_CTRL)] = A_SWAPGAME;

    ActionTable[SCOPE_OVERVIEW][GetKeyComboIndex(K_E, M_ALT)] = A_TOGGLE_EXPANSION;

    // Up, Down, Left, Right
    for (size_t i = K_LEFT; i < 4; i++) {
        ActionTable[SCOPE_SHEET][GetKeyComboIndex(i, M_NONE)] = A_NAVIGATELEFT + i;
    }

    // Placeholder for dummy action
    ActionTable[SCOPE_SHEET][UNUSED_KEY_COMBO] = A_DONOTHING;
}

#endif