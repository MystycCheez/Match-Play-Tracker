// #define GAP_DEBUG

#include "includes.h"

// #include "C:/msys64/ucrt64/include/rlgl.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);

    initGlobals();

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0, 0, "Match Play Tracker");
    Image WindowIcon = LoadImage("resources/logo-transparent.png");
    Texture2D WindowIconTexture = LoadTextureFromImage(WindowIcon);
    SetTextureFilter(WindowIconTexture, TEXTURE_FILTER_BILINEAR);
    SetWindowIcon(WindowIcon);

    // Vector2 scaleDPI = GetWindowScaleDPI();

    // UI.windowWidth = scaleDPI.x;

    // TODO: Find a way to account for the taskbar
    SetWindowSize(UI.windowWidth, UI.windowHeight);
    SetWindowPosition(
        (GetMonitorWidth(GetCurrentMonitor()) / 2) - (UI.windowWidth / 2), 
        (GetMonitorHeight(GetCurrentMonitor()) / 2) - (UI.windowHeight / 2)
    );

    loadSpecialText();

    Image TitleBarImage = LoadImage("resources/title-bar.png");
    Texture2D TitleBarTexture = LoadTextureFromImage(TitleBarImage);
    NPatchInfo TitleBarNPatch = {(Rectangle){0, 0, 1, TOP_BAR_HEIGHT}, 0, 0, 0, 0, NPATCH_THREE_PATCH_HORIZONTAL};
    Rectangle TitleBarRec = {0, 0, UI.windowWidth, TOP_BAR_HEIGHT};
    
    initButtons();

    SetExitKey(KEY_NULL);
    UI.font = loadFont();
    Players players = {"Player 1", "Player 2", 0, 0};
    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;
    int cursorTimer = 0;

    size_t selectedCellIndex = 0;

    Line borders[COLUMNS + ROWS] = {0};
    setBorderPositions(borders);

    Rectangle gradTop = {0, TOP_BAR_HEIGHT, UI.windowWidth, UI.cellHeight};
    Rectangle gradBot = {0, (UI.cellHeight * 21) + TOP_BAR_HEIGHT, UI.windowWidth, UI.cellHeight};

    Cell* sheet = initSheet(players, game);
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        InputHandler(sheet, &selectedCellIndex);
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        DrawTextureNPatch(TitleBarTexture, TitleBarNPatch, TitleBarRec, (Vector2){0, 0}, 0, WHITE);
        DrawTextureEx(GVARS.buttons[BTN_EXIT].texture, GVARS.buttons[BTN_EXIT].pos, 0, 2, getStateColor(GVARS.buttons[BTN_EXIT].state));
        DrawTextureEx(GVARS.buttons[BTN_MINIMIZE].texture, GVARS.buttons[BTN_MINIMIZE].pos, 0, 2, getStateColor(GVARS.buttons[BTN_MINIMIZE].state));
        DrawTexturePro(WindowIconTexture, (Rectangle){0, 0, 352, 352}, (Rectangle){3, 3, 44, 44}, (Vector2){0, 0}, 0, WHITE);
        
        // The following function is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx(gradTop, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx(gradBot, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        // Draw Cell Highlights for win/loss
        for (size_t i = 0; i < CELL_COUNT; i++)
            DrawRectangleV(indexToXY(i), (Vector2){UI.cellWidth, UI.cellHeight}, sheet[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        // Draw text highlighting
        if (GVARS.scope == SCOPE_CELL && GVARS.selection.exists) {
            DrawTextHighlight(sheet, selectedCellIndex);
        }
        // Draw Text :)
        BeginShaderMode(GVARS.shader);
        for (size_t i = 0; i < CELL_COUNT; i++) {
            DrawTextAligned(TextPos, sheet[i], i);
        }
        EndShaderMode();
        if (GVARS.scope >= SCOPE_SHEET) {
            DrawCellBorders(selectedCellIndex);
        }

        if (GVARS.scope == SCOPE_CELL) {
            if (cursorTimer % RefreshRate < RefreshRate / 2) {
                DrawCursor(sheet, selectedCellIndex);
            }
        }

        EndDrawing();
        cursorTimer++;
    }
    
    UnloadFont(UI.font);
    
    CloseWindow();
    UnloadImage(WindowIcon);
    
    return 0;
}