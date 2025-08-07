#ifndef INIT_C
#define INIT_C

#include "includes.h"
#include "GLFW/glfw3.h"

void initWindow()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0, 0, "Match Play Tracker");
    Window.Icon = LoadImage("resources/logo-transparent.png");
    Window.IconTexture = LoadTextureFromImage(Window.Icon);
    SetTextureFilter(Window.IconTexture, TEXTURE_FILTER_BILINEAR);
    SetWindowIcon(Window.Icon);

    Vector2 scaleDPI = GetWindowScaleDPI();

    Window.Width = Window.Width * scaleDPI.x;
    Window.Height = Window.Height * scaleDPI.y;

    // TODO: Find a way to account for the taskbar
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

void initGlobals()
{
    Window.Height = SCREEN_HEIGHT;
    Window.Width = SCREEN_WIDTH;
    UI.cellHeight = BASE_CELL_HEIGHT;
    UI.cellWidth = BASE_CELL_WIDTH;
    UI.fontSize = BASE_FONT_SIZE;
    GVARS.scope = SCOPE_SHEET;
    GVARS.shouldExit = false;
    GVARS.level_win = NULL;
}

void initButtons()
{
    UI.buttons = malloc(sizeof(Button) * 2);
    UI.buttons[BTN_EXIT].pos = (Vector2){Window.Width - BASE_BUTTON_SIZE - (BASE_BUTTON_SIZE / 2), (TOP_BAR_HEIGHT / 2) - 13};
    UI.buttons[BTN_EXIT].texture = LoadTextureFromImage(LoadImage("resources/x.png"));
    UI.buttons[BTN_EXIT].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_EXIT].texture, TEXTURE_FILTER_BILINEAR);
    UI.buttons[BTN_MINIMIZE].pos = (Vector2){Window.Width - (BASE_BUTTON_SIZE * 3), (TOP_BAR_HEIGHT / 2) - 13};
    UI.buttons[BTN_MINIMIZE].texture = LoadTextureFromImage(LoadImage("resources/minimize.png"));
    UI.buttons[BTN_MINIMIZE].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(UI.buttons[BTN_MINIMIZE].texture, TEXTURE_FILTER_BILINEAR);
}

void initSheetText(Cell *sheet, Players players, int game)
{
    char **levelText = loadLevelText(game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&sheet[(i * 3) + 3].gapStr, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%lld", players.s1);
    sprintf(s2, "%lld", players.s2);
    placeString(&sheet[0].gapStr, "Stage", CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 3].gapStr, "Points", CELL_TEXT_LENGTH);
    placeString(&sheet[1].gapStr, players.p1, CELL_TEXT_LENGTH);
    placeString(&sheet[2].gapStr, players.p2, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 2].gapStr, s1, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 1].gapStr, s2, CELL_TEXT_LENGTH);
}

Cell *initSheet(Players players, int game)
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
    initSheetText(sheet, players, game);
    return sheet;
}

void setBorderPositions(Line *borders)
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        borders[i] = (Line){0, (UI.cellHeight * i) + TOP_BAR_HEIGHT, Window.Width, (UI.cellHeight * i) + TOP_BAR_HEIGHT};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        borders[i + index] = (Line){UI.cellWidth * i, TOP_BAR_HEIGHT, UI.cellWidth * i, Window.Height};
    }
}

#endif