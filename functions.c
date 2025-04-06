#include "includes.h"

Font initFont()
{
    const char *font_file = "C:/Windows/Fonts/trebucbd.ttf";
    // Font font = LoadFontEx(font_file, FONT_SIZE, NULL, 250);

    int fileSize = 0;
    unsigned char* fileData = LoadFileData(font_file, &fileSize);
    Font font = {0};
    font.baseSize = FONT_SIZE * 5;
    font.glyphCount = 95;
    font.glyphs = LoadFontData(fileData, fileSize, FONT_SIZE * 5, 0, 0, FONT_SDF);
    Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, font.glyphCount, FONT_SIZE * 5, 0, 1);
    font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    // SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
    return font;
}

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
    pos.y = 1 + pos.y + (DEFAULT_CELL_HEIGHT * (index / 3));
    return pos;
}

void DrawCursor(Cell cell, unsigned int cellIndex, Font font)
{
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    Vector2 size = MeasureTextEx(font, cell.text, FONT_SIZE, 1);
    pos.x = pos.x + (DEFAULT_CELL_WIDTH / 2) + (size.x / 2);
    
    DrawLineEx(pos, (Vector2){pos.x, pos.y + DEFAULT_CELL_HEIGHT}, 1.0, GRAY);
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
            DrawTextLeftAligned(font, CalcTextPos(pos, i), fontSize, 1, cell);
            break;
        case ALIGN_CENTER:
            DrawTextCentered(font, CalcTextPos(pos, i), fontSize, 1, cell);
            break;
        case ALIGN_RIGHT:
            assert("TODO: ALIGN_RIGHT");
            break;
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
    }
}

// Expects format: "mm:ss"
unsigned int timeToSecs(char* time)
{
    size_t timeLen = strlen(time);
    assert(timeLen < 6 && "use filterCellText");
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

// Formats to mm:ss or 0:ss / Expects total number of seconds
char* secsToTime(unsigned int totalSecs)
{
    char* time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    unsigned int minutes = totalSecs / 60;
    assert(minutes < 100);
    unsigned int secs = totalSecs % 60;
    sprintf(time, "%d:%02d", minutes, secs);
    return time;
}

unsigned int countChars(char* text, char c, size_t len)
{
    unsigned int count = 0;
    char* p;
    p = memchr(text, c, len);
    while (p != NULL) {
        p = memchr(p+1, c, len);
        count++;
    }
    return count;
}

// Filters string to be converted into time / Outputs "mm:ss"
char* filterCellText(char* text)
{
    char* dummy = "00:00\0";
    size_t textLen = strlen(text);
    const char* tEnd = text + textLen - 1; // Don't want to refer to null terminator
    if (strpbrk(text, ":1234567890") != NULL) {
        unsigned int cCount = countChars(text, ':', textLen);
        if (cCount == 0) {
            if (textLen > 4 || textLen < 2) return dummy;
            return text;
        } else if (cCount == 1 && tEnd[-2] == ':') {
            if (textLen > 5 || textLen < 3) return dummy;
            return text;
        } else if (cCount == 2 && tEnd[-5] == ':') {
            if (textLen != 8) return dummy;
            return text;
        } else return dummy;
    } else return dummy;
}

void CompareTimes(Cell *cellL, Cell *cellR)
{
    unsigned int timeL = timeToSecs(filterCellText(cellL->text));
    unsigned int timeR = timeToSecs(filterCellText(cellR->text));
    if ((timeL == 0) || (timeR == 0)) return;
    if (timeL > timeR) {
        cellL->highlight = GREEN;
        cellR->highlight = RED;
    } else if (timeL < timeR) {
        cellL->highlight = RED;
        cellR->highlight = GREEN;
    } else if (timeL == timeR) {
        cellL->highlight = TRANSPARENT;
        cellR->highlight = TRANSPARENT;
    } // TODO: user gets WR as time and is highlighted GOLD (unlikely, but would like this feature)
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
    if (IsKeyPressed(KEY_ENTER)) {
        cell->text = filterCellText(cell->text);
        cell->cursor = strlen(cell->text);
    }
}