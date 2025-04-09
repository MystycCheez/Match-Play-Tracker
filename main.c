#include "includes.h"

int main()
{
    size_t epicLen = strlen("Hello,00000 World!") + 1;
    GapBuffer gapStr = initGapStr(epicLen);
    placeChar(&gapStr, 'H');
    placeChar(&gapStr, 'e');
    placeChar(&gapStr, 'l');
    placeChar(&gapStr, 'l');
    placeChar(&gapStr, 'o');
    cursorLeft(&gapStr);
    cursorLeft(&gapStr);
    cursorLeft(&gapStr);
    cursorLeft(&gapStr);
    cursorLeft(&gapStr);
    placeChar(&gapStr, 'X');
    placeChar(&gapStr, 'X');
    // cursorRight(&gapStr);
    // cursorRight(&gapStr);
    char* buf = gapStrToStr(gapStr, epicLen);
    printf("%s\n", buf);
    // printf("%lld - %lld\n", strlen(gapStr.str), strlen(gapStr.str + gapStr.cEnd + 1));
    // printf("%s\n", gapStr.str + gapStr.cEnd + 1);
    for (size_t i = 0; i < epicLen + 1; i++) {
        printf("%d ", gapStr.str[i]);
    }
    printf("\n");
    for (size_t i = 0; i < epicLen + 1; i++) {
        printf("%d ", buf[i]);
    }
    
    exit(0);
    char *test = malloc(sizeof("test :)"));
    sprintf(test, "test :)");
    char *test2 = malloc(sizeof("This is a "));
    sprintf(test2, "This is a ");
    size_t oldLen2 = strlen(test2);
    test2 = realloc(test2, oldLen2 + sizeof(char) * 5);
    memmove(test2 + oldLen2, test, 4);
    test2[strlen(test2)] = 0;
    printf("%s\n", test2);
    exit(0);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Match Play Tracker");

    Font font = initFont();
    Shader shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/sdf.fs", GLSL_VERSION));

    Players players = {"Player 1", "Player 2", 0, 0};

    unsigned int selectedCell = 4;
    bool selectionState = true;

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

    // GapBuffer gapBuf;
    // gapBuf.L = malloc(sizeof(char) * 0);
    // gapBuf.R = malloc(sizeof(char) * 0);

    Vector2 TextPos = {0};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        SelectionHandler(&selectionState, &selectedCell, &cell[selectedCell]);
        if (selectionState) InputHandler(cell, &selectedCell, &selectionState);
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        for (size_t i = 0; i < CELL_COUNT; i++) {
            DrawRectangleV(indexToXY(i), (Vector2){DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT}, cell[i].highlight);
        }
        // The following function is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx((Rectangle){0, DEFAULT_CELL_HEIGHT * 21, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx((Rectangle){0, 0, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT}, 
        GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        for (size_t i = 0; i < COLUMNS + ROWS; i++) {
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        }
        for (size_t i = 0; i < CELL_COUNT; i++) {
            BeginShaderMode(shader);
            DrawTextAligned(font, TextPos, FONT_SIZE, 1, cell[i], i);
            EndShaderMode();
        }    
        if (selectionState) DrawCursor(cell[selectedCell], selectedCell, font);
        DrawCellBorders(selectedCell);

        EndDrawing();
    }

    UnloadFont(font);

    CloseWindow();

    return 0;
}