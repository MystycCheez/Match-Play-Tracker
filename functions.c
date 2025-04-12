#include "includes.h"

Font initFont()
{
    const char *font_file = "C:/Windows/Fonts/trebucbd.ttf";
    // Font font = LoadFontEx(font_file, FONT_SIZE, NULL, 250);

    int fileSize = 0;
    unsigned char *fileData = LoadFileData(font_file, &fileSize);
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

char **loadLevelText()
{
    FILE *file_ptr = fopen("levels.txt", "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file: %s\n", "levels.txt");
        exit(1);
    }
    char **levelText = malloc(sizeof(char *) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        levelText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        memset(levelText[i], 0, CELL_TEXT_LENGTH);
    }
    char str[CELL_TEXT_LENGTH];
    size_t index = 0;
    while (fgets(str, CELL_TEXT_LENGTH, file_ptr) != NULL) {
        levelText[index] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        memset(levelText[index], 0, CELL_TEXT_LENGTH);
        strncpy(levelText[index], str, CELL_TEXT_LENGTH);
        memset(strchr(levelText[index], '\n'), 0, 1);
        index++;
    }

    fclose(file_ptr);
    return levelText;
}

bool isNotZero(int num)
{
    if (num > 0) return true;
    return false;
}

Vector2 indexToXY(unsigned int index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * DEFAULT_CELL_WIDTH;
    xy.y = (index / COLUMNS) * DEFAULT_CELL_HEIGHT;
    return xy;
}

Vector2 indexToCR(unsigned int index)
{
    Vector2 cr = {0};
    cr.x = index % COLUMNS;
    cr.y = index / COLUMNS;
    return cr;
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

unsigned int crToIndex(Vector2 cr)
{
    return (unsigned int)(cr.x + (cr.y * COLUMNS));
}

void initCellText(Cell *cell, Players players)
{
    char **levelText = loadLevelText();
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&cell[(i * 3) + 3].gapStr, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%d", players.s1);
    sprintf(s2, "%d", players.s2);
    placeString(&cell[0].gapStr, "Stage", CELL_TEXT_LENGTH);
    placeString(&cell[CELL_COUNT - 3].gapStr, "Points", CELL_TEXT_LENGTH);
    placeString(&cell[1].gapStr, players.p1, CELL_TEXT_LENGTH);
    placeString(&cell[2].gapStr, players.p2, CELL_TEXT_LENGTH);
    placeString(&cell[CELL_COUNT - 2].gapStr, s1, CELL_TEXT_LENGTH);
    placeString(&cell[CELL_COUNT - 1].gapStr, s2, CELL_TEXT_LENGTH);
}

void initBorderPositions(Line *borders)
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        borders[i] = (Line){0, DEFAULT_CELL_HEIGHT * i, SCREEN_WIDTH, DEFAULT_CELL_HEIGHT * i};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
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

// TODO: Fix!
void DrawCursor(Cell cell, unsigned int cellIndex, Font font)
{
    if (cellIndex == 0) return;
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    float span = MeasureTextEx(font, cell.gapStr.str, FONT_SIZE, 1).x;

    pos.x = pos.x + (DEFAULT_CELL_WIDTH / 2) + (span / 2);

    // TODO: Make cursor blink
    DrawLineEx(pos, (Vector2){pos.x, pos.y + DEFAULT_CELL_HEIGHT}, 1.0, GRAY);
}

void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + (DEFAULT_CELL_WIDTH / 2) - (size.x / 2);
    pos.y = pos.y + (DEFAULT_CELL_HEIGHT / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + fontSize / 2;
    pos.y = pos.y + (DEFAULT_CELL_HEIGHT / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
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
        assert(!"TODO: ALIGN_RIGHT");
        break;
    }
}

void SelectionHandler(bool *selectionState, unsigned int *cellIndex, Cell *cell)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, SELECTION_AREA)) {
            *selectionState = true;
            *cellIndex = xyToIndex(mousePos);
        } else {
            *selectionState = false;
            *cellIndex = 0;
            return;
        }
    }
    if (*cellIndex == 0) return;
    if (*cellIndex < (*cellIndex % 3)) {
        *selectionState = false;
        *cellIndex = 0;
    }
}

// Expects format: "mm:ss"
unsigned int timeToSecs(char *time)
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
char *secsToTime(unsigned int totalSecs)
{
    char *time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    unsigned int minutes = totalSecs / 60;
    assert(minutes < 100);
    unsigned int secs = totalSecs % 60;
    sprintf(time, "%d:%02d", minutes, secs);
    return time;
}

// Count how many times c appears in text limited by len
unsigned int countChars(char *text, char c, size_t len)
{
    unsigned int count = 0;
    char *p;
    p = memchr(text, c, len);
    while (p != NULL) {
        p = memchr(p + 1, c, len);
        count++;
    }
    return count;
}

// Filters string to be converted into time / Outputs "mm:ss"
char *filterText(char *text)
{
    // This is a mess of conditions, would be nice to make it more readable
    char *dummy = '\0';
    size_t textLen = strlen(text);
    // printf("text: %s textLen: %lld\n", text, textLen);
    if (strpbrk(text, ":1234567890") != NULL) {
        unsigned int cCount = countChars(text, ':', textLen);
        if (cCount == 0) {
            if (textLen > 4 || textLen < 2) return dummy;
            if (textLen == 3) {
                char *mod = malloc(sizeof(char) * 5);
                sprintf(mod, "%c:%s", text[0], text + 1);
                return mod;
            } else if (textLen == 4) {
                char *mod = malloc(sizeof(char) * 5);
                if (text[0] != '0') {
                    sprintf(mod, "%c%c:%s", text[0], text[1], text + 2);
                } else sprintf(mod, "%c:%s", text[1], text + 2);
                return mod;
            }
            else if (textLen == 2 && text[0] == '0') return dummy;
            char *mod = malloc(sizeof(char) * 5);
            sprintf(mod, "0:%s", text);
            return mod;
        } else if (cCount == 1 && text[textLen - 3] == ':') {
            if (textLen > 5 || textLen < 3) return dummy;
            if (textLen == 3) {
                char *mod = malloc(sizeof(char) * 5);
                sprintf(mod, "0%s", text);
                return mod;
            } else if (textLen == 4) {
                char *mod = malloc(sizeof(char) * 5);
                sprintf(mod, "%s", text);
                return mod;
            } else if ((textLen == 5) && (text[0] == '0')) {
                char *mod = malloc(sizeof(char) * 5);
                sprintf(mod, "%s", text + 1);
                return mod;
            }
            return text;
        } else if (cCount == 2 && text[textLen - 6] == ':') {
            if (textLen <= 8 && textLen >= 6) {
                char *mod = malloc(sizeof(char) * 5);
                sprintf(mod, "%s", text + textLen - 5);
                return filterText(mod);
            }
        } else return dummy;
    } else return dummy;
    return dummy;
}

Int2 CompareTimes(unsigned int row, Cell *cells)
{
    unsigned int cellL = crToIndex((Vector2){1, (float)row});
    unsigned int cellR = crToIndex((Vector2){2, (float)row});

    unsigned int timeL = timeToSecs(cells[cellL].gapStr.str);
    unsigned int timeR = timeToSecs(cells[cellR].gapStr.str);

    if (cells[cellL].gapStr.str[0] == 0 || cells[cellR].gapStr.str[0] == 0) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1};
    }

    if (timeL < timeR) {
        cells[cellL].highlight = COLOR_WIN;
        cells[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    else if (timeL > timeR) {
        cells[cellL].highlight = COLOR_LOSE;
        cells[cellR].highlight = COLOR_WIN;
        return (Int2){0, 1};
    }
    else if (timeL == timeR) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0};
    } // TODO: player gets WR as time and is highlighted GOLD (unlikely, but would like this feature) - UNTIEDS too
    assert(!"Shouldn't have reached here!");
    exit(-1);
}

void UpdateScores(Cell *cells)
{
    unsigned int tieCounter = 0;
    OverwriteStr(&cells[CELL_COUNT - 2].gapStr, "0", CELL_TEXT_LENGTH);
    OverwriteStr(&cells[CELL_COUNT - 1].gapStr, "0", CELL_TEXT_LENGTH);
    Int2 *wins = malloc(sizeof(Int2) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        wins[i] = CompareTimes(i + 1, cells);
        if (wins[i].a == 0 && wins[i].b == 0) {
            tieCounter++;
        }
        // if (wins[i].a == -1 || wins[i].b == -1) 
        if (wins[i].a > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str) + tieCounter), CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].b > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str) + tieCounter), CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].a > 0 || wins[i].b > 0){
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str)), CELL_TEXT_LENGTH);
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str)), CELL_TEXT_LENGTH);
        }
    }
}

void InputHandler(Cell *cellList, unsigned int *cellIndex, bool *selectionState, unsigned int *scoreTieAcc, bool *textChanged)
{
    Cell *cell = &cellList[*cellIndex];
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0) {
        // NOTE: Only allow keys in range [32..125]
        if ((key >= 32) && (key <= 125))
            placeChar(&cell->gapStr, (char)key, CELL_TEXT_LENGTH);
        key = GetCharPressed(); // Check next character in the queue
        *textChanged = true;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        cursorLeft(&cell->gapStr);
        *textChanged = true;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        cursorRight(&cell->gapStr, CELL_TEXT_LENGTH);
        *textChanged = true;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        deleteChar(&cell->gapStr);
        *textChanged = true;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (*cellIndex > 0 && *cellIndex < 3 && *textChanged == true) {;;} // TODO: Update Player Names
        if (*cellIndex > 2 && *cellIndex < CELL_COUNT - 6) {
            char *filteredText = filterText(cell->gapStr.str);
            if (*textChanged == true) {
                OverwriteStr(&cell->gapStr, filteredText, CELL_TEXT_LENGTH);
            }
            *cellIndex = *cellIndex + 3;
            free(filteredText);
        } else {
            *cellIndex = 0;
            selectionState = false;
        }
        if (*textChanged == true) UpdateScores(cellList);
        SelectionHandler(selectionState, cellIndex, cell);
    }
}