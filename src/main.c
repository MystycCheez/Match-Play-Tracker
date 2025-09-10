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
    
    initSheet();
    initSheetText();
    
    ExportActionTable();
    
    setBorderPositions();

    Rectangle gradTop = {0, UI.topBarHeight, Window.Width, UI.cellHeight};
    Rectangle gradBot = {0, (UI.cellHeight * 21) + UI.topBarHeight, Window.Width, UI.cellHeight};
    
    const int RefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    // const int RefreshRate = 10;
    SetTargetFPS(RefreshRate);
    
    while (!GVARS.shouldExit && !WindowShouldClose())
    {
        CellInputHandler();
        KeyHandler();
        // MouseHandler();
        MH();
        CursorHandler();
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        // Draw Title Bar
        DrawTextureNPatch(TitleBar.Texture, TitleBar.NPatch, TitleBar.Rec, (Vector2){0, 0}, 0, WHITE);
        DrawTextureEx(UI.buttons[BTN_EXIT].texture, UI.buttons[BTN_EXIT].pos, 0, 2, getStateColor(UI.buttons[BTN_EXIT].state));
        DrawTextureEx(UI.buttons[BTN_MINIMIZE].texture, UI.buttons[BTN_MINIMIZE].pos, 0, 2, getStateColor(UI.buttons[BTN_MINIMIZE].state));
        DrawTexturePro(Window.IconTexture, (Rectangle){0, 0, 352, 352}, (Rectangle){3, 3, 44, 44}, (Vector2){0, 0}, 0, WHITE);

        // Draw Debug Elements
        const char* d_pressed = TextFormat("p: %d", Mouse.pressed);
        const char* d_down = TextFormat("d: %d", Mouse.down);
        const char* d_released = TextFormat("r: %d", Mouse.released);
        Vector2 v_pressed = {MeasureTextEx(UI.font, d_pressed, UI.fontSize, 1).x + UI.fontSize * 2, 4};
        Vector2 v_down = {MeasureTextEx(UI.font, d_down, UI.fontSize, 1).x + v_pressed.x + UI.fontSize, 4};
        Vector2 v_released = {MeasureTextEx(UI.font, d_released, UI.fontSize, 1).x + v_down.x + UI.fontSize, 4};
        DrawTextEx(UI.font, d_pressed, v_pressed, UI.fontSize, 1, WHITE);
        DrawTextEx(UI.font, d_down, v_down, UI.fontSize, 1, WHITE);
        DrawTextEx(UI.font, d_released, v_released, UI.fontSize, 1, WHITE);
        
        // TODO: Make PR/Issue - DrawRectangleGradientEx() is documented incorrectly
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

    checkUnfreed();
    
    return 0;
}