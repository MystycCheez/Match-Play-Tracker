// #define GAP_DEBUG

#include "includes.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);
    SetExitKey(KEY_NULL);

    initGlobals();
    initWindow();
    initButtons();
    
    loadSpecialText();
    loadFont();

    Players players = {"Player 1", "Player 2", 0, 0};
    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;
    int cursorTimer = 0;

    size_t selectedCellIndex = 0;

    Line borders[COLUMNS + ROWS] = {0};
    setBorderPositions(borders);

    Rectangle gradTop = {0, TOP_BAR_HEIGHT, Window.Width, UI.cellHeight};
    Rectangle gradBot = {0, (UI.cellHeight * 21) + TOP_BAR_HEIGHT, Window.Width, UI.cellHeight};

    Cell* sheet = initSheet(players, game);
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        InputHandler(sheet, &selectedCellIndex);
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        DrawTextureNPatch(TitleBar.Texture, TitleBar.NPatch, TitleBar.Rec, (Vector2){0, 0}, 0, WHITE);
        DrawTextureEx(UI.buttons[BTN_EXIT].texture, UI.buttons[BTN_EXIT].pos, 0, 2, getStateColor(UI.buttons[BTN_EXIT].state));
        DrawTextureEx(UI.buttons[BTN_MINIMIZE].texture, UI.buttons[BTN_MINIMIZE].pos, 0, 2, getStateColor(UI.buttons[BTN_MINIMIZE].state));
        DrawTexturePro(Window.IconTexture, (Rectangle){0, 0, 352, 352}, (Rectangle){3, 3, 44, 44}, (Vector2){0, 0}, 0, WHITE);
        
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
    UnloadImage(Window.Icon);
    
    return 0;
}