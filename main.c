#include "includes.h"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Match Play Tracker");

    char *player1 = "Player 1";
    char *player2 = "Player 2";
    int score1 = 0;
    int score2 = 0;
    Players players = {player1, player2, score1, score2};

    unsigned int selectedCell = 4;
    bool selectionState = false;

    Line borders[COLUMNS + ROWS] = {0};
    initBorderPositions(borders);

    Cell cell[CELL_COUNT];
    for (size_t i = 0; i < CELL_COUNT; i++) {
        cell[i].cursor = 0;
        cell[i].text = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        memset(cell[i].text, 0, CELL_TEXT_LENGTH);
        cell[i].alignment = ALIGN_CENTER;
        cell[i].color = WHITE;
        cell[i].highlight = TRANSPARENT;
    }
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        cell[3 + (i * COLUMNS)].alignment = ALIGN_LEFT;
        cell[3 + (i * COLUMNS)].color = COLOR_LEVEL;
    }
    initCellText(cell, players);

    Vector2 TextPos = {0};

    const char *font_file = "C:/Windows/Fonts/trebucbd.ttf";
    Font font = LoadFontEx(font_file, 100, NULL, 250);

    SetTargetFPS(144);

    while (!WindowShouldClose())
    {
        SelectionHandler(&selectionState, &selectedCell);
        if (selectionState) InputHandler(&cell[selectedCell]);

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        // The following function is documented incorrectly
        for (size_t i = 0; i < CELL_COUNT; i++)
        {
            DrawRectangleV(indexToXY(i), (Vector2){DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT}, cell[i].highlight);
        }
        DrawRectangleGradientEx((Rectangle){0, DEFAULT_CELL_HEIGHT * 21, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx((Rectangle){0, 0, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        for (size_t i = 0; i < COLUMNS + ROWS; i++)
        {
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        }
        for (size_t i = 0; i < CELL_COUNT; i++)
        {
            DrawTextAligned(font, TextPos, 30, 1, cell[i], i);
        }    
        DrawCellBorders(selectedCell);

        EndDrawing();
    }

    UnloadFont(font);

    CloseWindow();

    return 0;
}