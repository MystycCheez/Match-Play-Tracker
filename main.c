#define GAP_DEBUG

#include "includes.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);
    
    GVARS.screenHeight = DEFAULT_SCREEN_HEIGHT;
    GVARS.screenWidth = DEFAULT_SCREEN_WIDTH;
    GVARS.cellHeight = DEFAULT_CELL_HEIGHT;
    GVARS.cellWidth = DEFAULT_CELL_WIDTH;
    GVARS.fontSize = DEFAULT_FONT_SIZE;
    GVARS.scope = SCOPE_SHEET;
    GVARS.level_win = NULL;
    GVARS.shouldExit = false;
    loadSpecialText();
    
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GVARS.screenWidth, GVARS.screenHeight, "Match Play Tracker");
    Image WindowIcon = LoadImage("resources/logo-transparent.png");
    Texture2D WindowIconTexture = LoadTextureFromImage(WindowIcon);
    SetTextureFilter(WindowIconTexture, TEXTURE_FILTER_BILINEAR);
    SetWindowIcon(WindowIcon);

    Image TitleBarImage = LoadImage("resources/title-bar.png");
    Texture2D TitleBarTexture = LoadTextureFromImage(TitleBarImage);
    NPatchInfo TitleBarNPatch = {(Rectangle){0, 0, 1, TOP_BAR_HEIGHT}, 0, 0, 0, 0, NPATCH_THREE_PATCH_HORIZONTAL};
    Rectangle TitleBarRec = {0, 0, GVARS.screenWidth, TOP_BAR_HEIGHT};
    
    initButtons();

    SetExitKey(KEY_NULL);
    Font font = loadFont();
    Players players = {"Player 1", "Player 2", 0, 0};
    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;
    int cursorTimer = 0;

    size_t selectedCellIndex = 0;

    Line borders[COLUMNS + ROWS] = {0};
    setBorderPositions(borders);

    Rectangle gradTop = {0, TOP_BAR_HEIGHT, GVARS.screenWidth, GVARS.cellHeight};
    Rectangle gradBot = {0, (GVARS.cellHeight * 21) + TOP_BAR_HEIGHT, GVARS.screenWidth, GVARS.cellHeight};

    Cell* sheet = initSheet(players, game);
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        InputHandler(sheet, &selectedCellIndex);
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        // TODO: Draw custom title bar
        DrawTextureNPatch(TitleBarTexture, TitleBarNPatch, TitleBarRec, (Vector2){0, 0}, 0, WHITE);
        DrawTextureEx(GVARS.buttons[BTN_EXIT].texture, GVARS.buttons[BTN_EXIT].pos, 0, 2, getStateColor(GVARS.buttons[BTN_EXIT].state));
        DrawTextureEx(GVARS.buttons[BTN_MINIMIZE].texture, GVARS.buttons[BTN_MINIMIZE].pos, 0, 2, getStateColor(GVARS.buttons[BTN_MINIMIZE].state));
        DrawTexturePro(WindowIconTexture, (Rectangle){0, 0, 352, 352}, (Rectangle){3, 3, 44, 44}, (Vector2){0, 0}, 0, WHITE);
        
        // The following function is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx(gradTop, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx(gradBot, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        // Draw Cell Highlights for win/loss
        for (size_t i = 0; i < CELL_COUNT; i++)
            DrawRectangleV(indexToXY(i), (Vector2){GVARS.cellWidth, GVARS.cellHeight}, sheet[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        // Draw text highlighting
        if (GVARS.scope == SCOPE_CELL && GVARS.selection.exists) {
            DrawTextHighlight(sheet, selectedCellIndex, font);
        }
        // Draw Text :)
        for (size_t i = 0; i < CELL_COUNT; i++) 
            DrawTextAligned(font, TextPos, GVARS.fontSize, 1, sheet[i], i);

        if (GVARS.scope >= SCOPE_SHEET) {
            DrawCellBorders(selectedCellIndex);
        }

        if (GVARS.scope == SCOPE_CELL) {
            if (cursorTimer % RefreshRate < RefreshRate / 2) {
                DrawCursor(sheet, selectedCellIndex, font);
            }
        }

        EndDrawing();
        cursorTimer++;
    }

    // ExportToBBCode(sheet);
    // saveTimes(sheet);
    
    UnloadFont(font);
    
    CloseWindow();
    UnloadImage(WindowIcon);
    
    return 0;
}