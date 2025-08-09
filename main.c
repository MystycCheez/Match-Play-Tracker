// #define GAP_DEBUG

#include "includes.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);
    
    initWindow();
    initGlobals();
    initButtons();
    
    loadSpecialText();
    loadFont();

    SetExitKey(KEY_NULL);

    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;

    size_t selectedCellIndex = 0;

    setBorderPositions(UI.borders);

    Rectangle gradTop = {0, TOP_BAR_HEIGHT, Window.Width, UI.cellHeight};
    Rectangle gradBot = {0, (UI.cellHeight * 21) + TOP_BAR_HEIGHT, Window.Width, UI.cellHeight};

    Cell* sheet = initSheet(game);
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        InputHandler(sheet, &selectedCellIndex);
        CursorHandler(selectedCellIndex);
        
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
            DrawRectangleV(indexToXY(i), (Vector2){UI.cellWidth, UI.cellHeight}, sheet[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(UI.borders[i].x1, UI.borders[i].y1, UI.borders[i].x2, UI.borders[i].y2, BORDER_COLOR);
        // Draw text highlighting
        if (GVARS.scope == SCOPE_CELL && GVARS.selection.exists) {
            DrawTextHighlight(sheet, selectedCellIndex);
        }
        // Draw Text :)
        for (size_t i = 0; i < CELL_COUNT; i++) {
            DrawTextAligned(TextPos, sheet[i], i);
        }
        if (GVARS.scope >= SCOPE_SHEET) {
            DrawSelectionBorders(selectedCellIndex);
        }
        // Draw blinking cursor
        if (GVARS.scope == SCOPE_CELL) {
            if (Cursor.timer % RefreshRate < RefreshRate / 2) {
                DrawCursor(sheet, selectedCellIndex);
            }
        }

        EndDrawing();
        Cursor.timer++;
    }
    
    CloseWindow();
    UnloadFont(UI.font);
    UnloadImage(Window.Icon);
    
    return 0;
}