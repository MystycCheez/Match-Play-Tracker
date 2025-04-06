#include "includes.h"

int main()
{
    // printf("%s\n", secsToTime(69));
    // filterCellText("0:00");
    printf("01:11 -> %d\n", timeToSecs(filterCellText("01:11")));
    printf("1:11  -> %d\n", timeToSecs(filterCellText("1:11")));
    printf(":11   -> %d\n", timeToSecs(filterCellText(":11")));
    printf("11    -> %d\n", timeToSecs(filterCellText("11")));
    printf("1     -> %d\n", timeToSecs(filterCellText("1"))); // Only this should fail
    exit(1);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Match Play Tracker");

    Font font = initFont();
    Shader shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/sdf.fs", GLSL_VERSION));

    Players players = {"Player 1", "Player 2", 0, 0};

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
            BeginShaderMode(shader);
            DrawTextAligned(font, TextPos, FONT_SIZE, 1, cell[i], i);
            EndShaderMode();
        }    
        DrawCellBorders(selectedCell);

        EndDrawing();
    }

    UnloadFont(font);

    CloseWindow();

    return 0;
}