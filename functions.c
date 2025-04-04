#include "includes.h"

char** loadLevelText()
{
    FILE *file_ptr = fopen("levels.txt", "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file: %s\n", "levels.txt");
        exit(1);
    }
    char** levelText = malloc(sizeof(char*) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        levelText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        memset(levelText[i], 0, CELL_TEXT_LENGTH);
    }
    char str[CELL_TEXT_LENGTH];
    size_t index = 0;
    while (fgets(str, CELL_TEXT_LENGTH, file_ptr) != NULL)
    {
        levelText[index] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        memset(levelText[index], 0, CELL_TEXT_LENGTH);
        strncpy(levelText[index], str, CELL_TEXT_LENGTH);
        memset(strchr(levelText[index], '\n'), 0, 1);
        index++;
    }

    fclose(file_ptr);
    return levelText;
}

Vector2 indexToXY(unsigned int index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * DEFAULT_CELL_WIDTH;
    xy.y = (index / COLUMNS) * DEFAULT_CELL_HEIGHT;
    return xy;
}

unsigned int xyToIndex(Vector2 xy)
{
    unsigned int index = 0;
    Vector2 rounded = {xy.x - fmodf(xy.x, (float)DEFAULT_CELL_WIDTH), xy.y - fmodf(xy.y, (float)DEFAULT_CELL_HEIGHT)};
    rounded.x = rounded.x / DEFAULT_CELL_WIDTH;
    rounded.y = rounded.y / DEFAULT_CELL_HEIGHT;
    index = rounded.x + (rounded.y * COLUMNS);
    return index;
}

void initCellText(Cell* cell, Players players)
{
    char** levelText = loadLevelText();
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        strncpy(cell[(i * 3) + 3].text, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    cell[0].text = "Stage";
    cell[CELL_COUNT - 3].text = "Points"; 
    sprintf(cell[1].text, "%s", players.p1);
    sprintf(cell[2].text, "%s", players.p2);
    sprintf(cell[CELL_COUNT - 2].text, "%d", players.s1);
    sprintf(cell[CELL_COUNT - 1].text, "%d", players.s2);
}

void initBorderPositions(Line* borders)
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++)
    {
        borders[i] = (Line){0, DEFAULT_CELL_HEIGHT * i, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT * i};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++)
    {
        borders[i + index] = (Line){DEFAULT_CELL_WIDTH * i, 0, DEFAULT_CELL_WIDTH * i, SCREEN_HEIGHT};
    }
}

void DrawCellBorders(unsigned int cellIndex)
{
    if (cellIndex == 0) return;
    Vector2 cellOrigin = indexToXY(cellIndex);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT}, 3.0, RAYWHITE);
}

Vector2 CalcTextPos(Vector2 pos, size_t index)
{
    pos.x = pos.x + (DEFAULT_CELL_WIDTH * (index % 3));
    pos.y = pos.y + (DEFAULT_CELL_HEIGHT * (index / 3));
    return pos;
}

void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    Vector2 size = MeasureTextEx(font, cell.text, fontSize, spacing);
    pos.x = pos.x + (DEFAULT_CELL_WIDTH / 2) - (size.x / 2);
    pos.y = pos.y + (DEFAULT_CELL_HEIGHT / 2) - (size.y / 2);

    DrawTextEx(font, cell.text, pos, fontSize, spacing, cell.color);
}

void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    Vector2 size = MeasureTextEx(font, cell.text, fontSize, spacing);
    pos.x = pos.x + fontSize / 2;
    pos.y = pos.y + DEFAULT_CELL_HEIGHT / 2 - (size.y / 2);

    DrawTextEx(font, cell.text, pos, fontSize, spacing, cell.color);
}

void DrawTextAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell, size_t i)
{
    switch (cell.alignment) {
        case ALIGN_LEFT:
            DrawTextLeftAligned(font, CalcTextPos(pos, i), 30, 1, cell);
            break;
        case ALIGN_CENTER:
            DrawTextCentered(font, CalcTextPos(pos, i), 30, 1, cell);
            break;
        case ALIGN_RIGHT:
            assert("TODO: ALIGN_RIGHT");
            break;
        }
}

// Expects format: mmss
unsigned int timeToSecs(char* time)
{
    size_t timeLen = strlen(time);
    assert(timeLen > 4);
    if (timeLen < 3) {
        return atoi(time);
    } else {
        char *a = malloc(sizeof(char) * timeLen - 2);
        char *b = malloc(sizeof(char) * 2);
        strncpy(a, time, timeLen - 2);
        strcpy(b, time + timeLen - 2);
        unsigned int minutes = atoi(a);
        unsigned int seconds = atoi(b);
        return seconds + (minutes * 60);
    }
}

// Formats to mm:ss or 0:ss
char* secsToTime(unsigned int totalSecs)
{
    char* time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    unsigned int minutes = totalSecs / 60;
    assert(minutes < 100);
    unsigned int secs = totalSecs % 60;
    sprintf(time, "%d:%d", minutes, secs);
    printf("%s\n", time);
    return time;
}

void InputHandler(Cell *cell)
{
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0) {
        // NOTE: Only allow keys in range [32..125]
        if ((key >= 32) && (key <= 125) && (cell->cursor < CELL_TEXT_LENGTH - 1)) {
            cell->text[cell->cursor] = (char)key;
            cell->text[cell->cursor + 1] = '\0'; // Add null terminator at the end of the string.
            cell->cursor++;
        }

        key = GetCharPressed(); // Check next character in the queue
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        cell->cursor--;
        if (cell->cursor < 0)
            cell->cursor = 0;
        cell->text[cell->cursor] = '\0';
    }
}

void SelectionHandler(bool *selectionState, unsigned int *cellIndex)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, SELECTION_AREA)) {
            *selectionState = true;
        } else {
            *selectionState = false;
            *cellIndex = 0;
            return;
        }
        *cellIndex = xyToIndex(mousePos);
        printf("%d\n", *cellIndex);
    }
}

char* filterCellText(char* text, unsigned int mode)
{
    if (mode == MODE_TIME) {
        if (strchr(text, ':') != NULL) {
            if (strlen(text) > 5) {
                memset(text, 0, CELL_TEXT_LENGTH);
                return text;
            }
        }
    }
    return text;
}

void CompareTimes(Cell cellL, Cell cellR)
{
    unsigned int timeL = timeToSecs(filterCellText(cellL.text, MODE_TIME));
    unsigned int timeR = timeToSecs(filterCellText(cellR.text, MODE_TIME));
    if (timeL > timeR) cellL.highlight = GREEN;
}