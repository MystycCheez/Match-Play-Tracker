// #define GAP_DEBUG
// #define ACTION_DEBUG

#include <stdlib.h>

#include "headers.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);
    
    initWindow();
    initGlobals();
    initButtons();
    
    loadSpecialText();
    loadFont();

    initActionTable();

    SetExitKey(KEY_NULL);

    initSheet();

    ExportActionTable();

    setBorderPositions();

    Rectangle gradTop = {0, UI.topBarHeight, Window.Width, UI.cellHeight};
    Rectangle gradBot = {0, (UI.cellHeight * 21) + UI.topBarHeight, Window.Width, UI.cellHeight};
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        InputHandler();
        CursorHandler();
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        DrawTextureNPatch(TitleBar.Texture, TitleBar.NPatch, TitleBar.Rec, (Vector2){0, 0}, 0, WHITE);
        DrawTextureEx(UI.buttons[BTN_EXIT].texture, UI.buttons[BTN_EXIT].pos, 0, 2, getStateColor(UI.buttons[BTN_EXIT].state));
        DrawTextureEx(UI.buttons[BTN_MINIMIZE].texture, UI.buttons[BTN_MINIMIZE].pos, 0, 2, getStateColor(UI.buttons[BTN_MINIMIZE].state));
        DrawTexturePro(Window.IconTexture, (Rectangle){0, 0, 352, 352}, (Rectangle){3, 3, 44, 44}, (Vector2){0, 0}, 0, WHITE);
        
        // DrawRectangleGradientEx() is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx(gradTop, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx(gradBot, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        // Draw Cell Highlights for win/loss
        for (size_t i = 0; i < CELL_COUNT; i++)
            DrawRectangleV(indexToXY(i), (Vector2){UI.cellWidth, UI.cellHeight}, Sheet.cellList[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(UI.borders[i].x1, UI.borders[i].y1, UI.borders[i].x2, UI.borders[i].y2, BORDER_COLOR);
        // Draw text highlighting
        if (GVARS.scope == SCOPE_CELL && Sheet.selection.exists) {
            DrawTextHighlight();
        }
        // Draw Text :)
        for (size_t i = 0; i < CELL_COUNT; i++) {
            DrawTextAligned(UI.TextPos, Sheet.cellList[i], i);
        }
        if (GVARS.scope >= SCOPE_SHEET) {
            DrawSelectionBorders();
        }
        // Draw blinking cursor
        if (GVARS.scope == SCOPE_CELL) {
            if (Cursor.timer % RefreshRate < RefreshRate / 2) {
                DrawCursor();
            }
        }

        EndDrawing();
        Cursor.timer++;
    }
    
    CloseWindow();
    CleanUp();
    
    return 0;
}