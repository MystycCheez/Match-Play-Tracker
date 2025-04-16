// #define GAP_DEBUG

#include "includes.h"

int main()
{
    GVARS.screenHeight = DEFAULT_SCREEN_HEIGHT;
    GVARS.screenWidth = DEFAULT_SCREEN_WIDTH;
    GVARS.cellHeight = DEFAULT_CELL_HEIGHT;
    GVARS.cellWidth = DEFAULT_CELL_WIDTH;
    GVARS.fontSize = DEFAULT_FONT_SIZE;
    GVARS.SelectionArea = initSelectionArea();
    loadSpecialText();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GVARS.screenWidth, GVARS.screenHeight, "Match Play Tracker");
    Image WindowIcon = LoadImage("resources/logo-transparent.png");
    SetWindowIcon(WindowIcon);

    SetExitKey(KEY_NULL);
    Font font = initFont();
    Players players = {"Player 1", "Player 2", 0, 0};
    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;
    int cursorTimer = 0;
    size_t scoreTieAcc = 0;

    size_t selectedCell = 0;
    bool selectionState = false;
    bool textChanged = false;

    Line borders[COLUMNS + ROWS] = {0};
    initBorderPositions(borders);

    Cell cell[CELL_COUNT];
    for (size_t i = 0; i < CELL_COUNT; i++) {
        cell[i].gapStr = initGapStr(CELL_TEXT_LENGTH);
        cell[i].alignment = ALIGN_CENTER;
        cell[i].color = WHITE;
        cell[i].highlight = TRANSPARENT;
        cell[i].hasTime = false;
    }
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        cell[3 + (i * COLUMNS)].alignment = ALIGN_LEFT;
        cell[3 + (i * COLUMNS)].color = COLOR_LEVEL;
    }
    initCellText(cell, players, game);
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            reInitGVARS();
            initBorderPositions(borders);
        }
        InputHandler(cell, &selectedCell, &selectionState, &scoreTieAcc, &textChanged);
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        
        // The following function is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx((Rectangle){0, GVARS.cellHeight * 21, GVARS.screenWidth, GVARS.cellHeight}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx((Rectangle){0, 0, GVARS.screenWidth, GVARS.cellHeight}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        // Draw Cell Highlights for win/loss
        for (size_t i = 0; i < CELL_COUNT; i++)
            DrawRectangleV(indexToXY(i), (Vector2){GVARS.cellWidth, GVARS.cellHeight}, cell[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        // Draw Text :)
        for (size_t i = 0; i < CELL_COUNT; i++) 
            DrawTextAligned(font, TextPos, GVARS.fontSize, 1, cell[i], i);

        if (selectionState && cursorTimer % 60 < 30) {
            DrawCursor(cell[selectedCell], selectedCell, font);
        }
        DrawCellBorders(selectedCell);
        
        EndDrawing();
        cursorTimer++;
    }
    
    UnloadFont(font);
    
    CloseWindow();
    UnloadImage(WindowIcon);
    
    return 0;
}