// #define GAP_DEBUG

#include "includes.h"

int main()
{
    GVARS.screenHeight = DEFAULT_SCREEN_HEIGHT;
    GVARS.screenWidth = DEFAULT_SCREEN_WIDTH;
    GVARS.cellHeight = DEFAULT_CELL_HEIGHT;
    GVARS.cellWidth = DEFAULT_CELL_WIDTH;
    GVARS.fontSize = DEFAULT_FONT_SIZE;
    GVARS.shouldExit = false;
    loadSpecialText();
    
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(GVARS.screenWidth, GVARS.screenHeight, "Match Play Tracker");
    Image WindowIcon = LoadImage("resources/logo-transparent.png");
    SetWindowIcon(WindowIcon);
    
    Image TitleBarImage = LoadImage("resources/title-bar.png");
    Texture2D TitleBarTexture = LoadTextureFromImage(TitleBarImage);
    NPatchInfo TitleBarNPatch = {(Rectangle){0, 0, 1, TOP_BAR_HEIGHT}, 0, 0, 0, 0, NPATCH_THREE_PATCH_HORIZONTAL};
    Rectangle TitleBarRec = {0, 0, GVARS.screenWidth, TOP_BAR_HEIGHT};
    
    initButtons();

    SetExitKey(KEY_NULL);
    Font font = initFont();
    Players players = {"Player 1", "Player 2", 0, 0};
    Vector2 TextPos = {0}; 

    int game = LEVELS_GE;
    int cursorTimer = 0;

    size_t selectedCell = 0;
    bool selectionState = false;
    bool textChanged = false;

    Line borders[COLUMNS + ROWS] = {0};
    setBorderPositions(borders);

    Rectangle gradTop = {0, TOP_BAR_HEIGHT, GVARS.screenWidth, GVARS.cellHeight};
    Rectangle gradBot = {0, (GVARS.cellHeight * 21) + TOP_BAR_HEIGHT, GVARS.screenWidth, GVARS.cellHeight};

    Cell* sheet = initSheet(players, game);

    // size_t index = 0;
    
    SetTargetFPS(60);
    
    while (!GVARS.shouldExit)
    {
        InputHandler(sheet, &selectedCell, &selectionState, &textChanged);
        if (IsWindowResized()) {
            reInitGVARS();
            setBorderPositions(borders);
            UpdateRects(&gradTop, &gradBot);
        }
        
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        // TODO: Draw custom title bar
        DrawTextureNPatch(TitleBarTexture, TitleBarNPatch, TitleBarRec, (Vector2){0, 0}, 0, WHITE);
        // DrawTextureEx(DownTexture, (Vector2){GVARS.screenWidth / 2, (73 / 2) - 12}, 0, 2, WHITE);
        DrawTextureEx(GVARS.buttons[BTN_EXIT].textures[STATE_BTN_UNHIGHLIGHTED], GVARS.buttons[BTN_EXIT].pos, 0, 2, WHITE);
        DrawTextureEx(GVARS.buttons[BTN_MINIMIZE].textures[STATE_BTN_UNHIGHLIGHTED], GVARS.buttons[BTN_MINIMIZE].pos, 0, 2, WHITE);
        
        // The following function is documented incorrectly - TODO: Make PR/Issue
        DrawRectangleGradientEx(gradTop, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        DrawRectangleGradientEx(gradBot, GRADIENT_TOP, GRADIENT_BOTTOM, GRADIENT_BOTTOM, GRADIENT_TOP);
        // Draw Cell Highlights for win/loss
        for (size_t i = 0; i < CELL_COUNT; i++)
            DrawRectangleV(indexToXY(i), (Vector2){GVARS.cellWidth, GVARS.cellHeight}, sheet[i].highlight);
        // Draw Borders
        for (size_t i = 0; i < COLUMNS + ROWS; i++) 
            DrawLine(borders[i].x1, borders[i].y1, borders[i].x2, borders[i].y2, BORDER_COLOR);
        // Draw Text :)
        for (size_t i = 0; i < CELL_COUNT; i++) 
            DrawTextAligned(font, TextPos, GVARS.fontSize, 1, sheet[i], i);

        if (selectionState) {
            if (cursorTimer % 60 < 30) {
                DrawCursor(sheet[selectedCell], selectedCell, font);
            }
            DrawCellBorders(selectedCell);
        }

        if (CheckCollisionPointRec(GetMousePosition(), getButtonRect(GVARS.buttons[BTN_EXIT]))) {
            DrawRectangleRec(getButtonRect(GVARS.buttons[BTN_EXIT]), WHITE);
        }
        if (CheckCollisionPointRec(GetMousePosition(), getButtonRect(GVARS.buttons[BTN_MINIMIZE]))) {
            DrawRectangleRec(getButtonRect(GVARS.buttons[BTN_MINIMIZE]), WHITE);
        }
        
        EndDrawing();
        cursorTimer++;
    }
    
    UnloadFont(font);
    
    CloseWindow();
    UnloadImage(WindowIcon);
    
    return 0;
}