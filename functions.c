#include "includes.h"

Font initFont()
{
    const char *font_file = "C:/Windows/Fonts/trebucbd.ttf";
    int fileSize = 0;
    unsigned char *fileData = LoadFileData(font_file, &fileSize);
    Font font = {0};
    font.baseSize = GVARS.fontSize * 5;
    font.glyphCount = 95;
    font.glyphs = LoadFontData(fileData, fileSize, GVARS.fontSize * 5, 0, 0, FONT_SDF);
    Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, font.glyphCount, GVARS.fontSize * 5, 0, 1);
    font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    // SetTextureFilter(font.texture, TEXTURE_FILTER_POINT); // Will I need this?
    return font;
}

// What cells can the user select (not the special text like stage names/points)
Rectangle initSelectionArea()
{
    return (Rectangle){GVARS.cellWidth, 0, GVARS.cellWidth * 2, (GVARS.cellHeight * 21)};
}

void reInitGVARS()
{
    GVARS.screenHeight = GetScreenHeight();
    GVARS.screenWidth = GetScreenWidth();
    GVARS.cellHeight = GVARS.screenHeight / ROWS;
    GVARS.cellWidth = GVARS.screenWidth / COLUMNS;
    GVARS.fontSize = GVARS.screenHeight / 44;
    GVARS.SelectionArea = initSelectionArea();
}

char **loadLevelText(int game)
{
    char* filename;
    if (game == LEVELS_GE) filename = "resources/levels-ge.txt";
    if (game == LEVELS_PD) filename = "resources/levels-pd.txt";
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file: %s\n", filename);
        printf("%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    char **levelText = malloc(sizeof(char*) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        levelText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        fgets(levelText[i], CELL_TEXT_LENGTH, file_ptr);
        memset(strchr(levelText[i], '\n'), 0, 1);
    }

    fclose(file_ptr);
    return levelText;
}

void loadSpecialText()
{
    FILE *file_ptr = fopen("resources/specials.txt", "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file: %s\n", "specials.txt");
        printf("%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    size_t specialCount = 0;
    char* tmp = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    while (fgets(tmp, CELL_TEXT_LENGTH, file_ptr) != NULL) specialCount++;
    free(tmp);
    rewind(file_ptr);
    char **specialText = malloc(sizeof(char*) * specialCount);
    for (size_t i = 0; i < specialCount; i++) {
        specialText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        fgets(specialText[i], CELL_TEXT_LENGTH, file_ptr);
        memset(strchr(specialText[i], '\n'), 0, 1);
    }
    GVARS.specials.text = specialText;
    GVARS.specials.count = specialCount;
}

// TODO
int CompareSpecialText(char* text)
{
    int returnVal = -1;
    for (size_t i = 0; i < GVARS.specials.count; i++) {
        if (i > 0 && i < 4) returnVal = TEXT_VETO;
        if (i > 3)          returnVal = TEXT_DNF;
        if (strcmp(text, GVARS.specials.text[i]) == 0) return returnVal;
    }
    return returnVal;
}

bool isNotZero(int num)
{
    if (num > 0) return true;
    return false;
}

// XY being the top left corner of the cell index
Vector2 indexToXY(size_t index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * GVARS.cellWidth;
    xy.y = (index / COLUMNS) * GVARS.cellHeight;
    return xy;
}

// CR being column/row
Vector2 indexToCR(size_t index)
{
    Vector2 cr = {0};
    cr.x = index % COLUMNS;
    cr.y = index / COLUMNS;
    return cr;
}

// XY being anywhere within a cell
size_t xyToIndex(Vector2 xy)
{
    size_t index = 0;
    Vector2 rounded = {xy.x - fmodf(xy.x, GVARS.cellWidth), xy.y - fmodf(xy.y, GVARS.cellHeight)};
    rounded.x = rounded.x / GVARS.cellWidth;
    rounded.y = rounded.y / GVARS.cellHeight;
    index = rounded.x + (rounded.y * COLUMNS);
    return index;
}

// CR being column/row
size_t crToIndex(Vector2 cr)
{
    return (size_t)(cr.x + (cr.y * COLUMNS));
}

void initCellText(Cell *cell, Players players, int game)
{
    char **levelText = loadLevelText(game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&cell[(i * 3) + 3].gapStr, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%lld", players.s1);
    sprintf(s2, "%lld", players.s2);
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
        borders[i] = (Line){0, GVARS.cellHeight * i, GVARS.screenWidth, GVARS.cellHeight * i};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        borders[i + index] = (Line){GVARS.cellWidth * i, 0, GVARS.cellWidth * i, GVARS.screenHeight};
    }
}

void DrawCellBorders(size_t cellIndex)
{
    if (cellIndex == 0) return;
    Vector2 cellOrigin = indexToXY(cellIndex);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y, GVARS.cellWidth, GVARS.cellHeight}, 3.0, RAYWHITE);
}

// TODO: check if this can utilize xy/cr functions
Vector2 CalcTextPos(Vector2 pos, size_t index)
{
    pos.x = pos.x + (GVARS.cellWidth * (index % 3));
    pos.y = 1 + pos.y + (GVARS.cellHeight * (index / 3));
    return pos;
}

// TODO: Fix! // Fix what??
void DrawCursor(Cell cell, size_t cellIndex, Font font)
{
    if (cellIndex == 0) return;
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    float span = MeasureTextEx(font, cell.gapStr.str, GVARS.fontSize, 1).x;
    pos.x = pos.x + (GVARS.cellWidth / 2) + (span / 2);
    DrawLineEx(pos, (Vector2){pos.x, pos.y + GVARS.cellHeight}, 1.0, GRAY);
}

void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + (GVARS.cellWidth / 2) - (size.x / 2);
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + fontSize / 2;
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

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

// Expects format: "mm:ss"
size_t timeToSecs(char *time)
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
        size_t minutes = atoi(a);
        size_t seconds = atoi(b);
        return seconds + (minutes * 60);
    }
}

// Formats to mm:ss or 0:ss / Expects total number of seconds
char *secsToTime(size_t totalSecs)
{
    char *time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    size_t minutes = totalSecs / 60;
    assert(minutes < 100);
    size_t secs = totalSecs % 60;
    sprintf(time, "%lld:%02lld", minutes, secs);
    return time;
}

// Count how many times char c appears in text limited by len
size_t countChars(char *text, char c, size_t len)
{
    size_t count = 0;
    char *p;
    p = memchr(text, c, len);
    while (p != NULL) {
        p = memchr(p + 1, c, len);
        count++;
    }
    return count;
}

// i.e. "veto" or "dnf"
int HasSpecialText(Cell cell)
{   
    // Surely I can put these in a text file and read them
    if (strcmp(cell.gapStr.str, "veto") == 0 ||
        strcmp(cell.gapStr.str, "Veto") == 0 ||
        strcmp(cell.gapStr.str, "VETO") == 0 ||
        strcmp(cell.gapStr.str, "-")    == 0 || // Should it be veto?
        strcmp(cell.gapStr.str, "dnf")  == 0 ||
        strcmp(cell.gapStr.str, "DNF")  == 0 ||
        strcmp(cell.gapStr.str, "Dnf")  == 0) {
            return true;
        } else return false;
}

// Filters string to be converted into time / Outputs "mm:ss" or "m:ss"
char *filterText(char *text)
{
    char *text_veto = malloc(sizeof("Veto"));
    char *text_dnf  = malloc(sizeof("DNF"));
    strcpy(text_veto, "Veto");
    strcpy(text_dnf, "DNF");

    // if (getSpecialText(text) == TEXT_VETO) return text_veto;
    
    // This is a mess of conditions, would be nice to make it more readable
    char *dummy = "\0";
    size_t textLen = strlen(text);
    // printf("text: %s textLen: %lld\n", text, textLen);  
    if (strpbrk(text, ":1234567890") != NULL) {
        size_t cCount = countChars(text, ':', textLen);
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

// Compares two cells in a row and returns a pair of ints representing who won or lost
// Also checks for special cases
Int2 CompareTimes(size_t row, Cell *cells)
{
    size_t cellL = crToIndex((Vector2){1, (float)row});
    size_t cellR = crToIndex((Vector2){2, (float)row});

    size_t timeL = timeToSecs(cells[cellL].gapStr.str);
    size_t timeR = timeToSecs(cells[cellR].gapStr.str);

    int specialL = CompareSpecialText(cells[cellL].gapStr.str);
    int specialR = CompareSpecialText(cells[cellR].gapStr.str);

    if (cells[cellL].gapStr.str[0] == 0 || cells[cellR].gapStr.str[0] == 0) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1};
    }

    if ((timeL < timeR) || (specialL != -1 && specialR > 0)) {
        cells[cellL].highlight = COLOR_WIN;
        cells[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    if ((timeL > timeR) || (specialR != -1 && specialL > 0)) {
        cells[cellL].highlight = COLOR_LOSE;
        cells[cellR].highlight = COLOR_WIN;
        return (Int2){0, 1};
    }
    if ((timeL == timeR) || (specialL > 0 && specialR > 0)) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0};
    } // TODO: player gets WR as time and is highlighted GOLD (unlikely, but would like this feature) - UNTIEDS too
    // printf("%s:%d\n", __FILE__, __LINE__);
    assert(!"Shouldn't have reached here!");
    exit(-1);
}

// Compares times in each row and calculates the scores, overwriting the text
void UpdateScores(Cell *cells)
{
    size_t tieCounter = 0;
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

// This is a big function, Should I break it down?
void InputHandler(Cell *cellList, size_t *cellIndex, bool *selectionState, size_t *scoreTieAcc, bool *textChanged)
{
    Cell *cell = &cellList[*cellIndex];
    int key_char = GetCharPressed();
    int key_key = GetKeyPressed();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, GVARS.SelectionArea)) {
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

    if (key_key == KEY_ESCAPE) {
        *selectionState = false;
        *cellIndex = 0;
    }
    if (*selectionState == true) {
        // Check if more characters have been pressed on the same frame
        while (key_char > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key_char >= 32) && (key_char <= 125))
                placeChar(&cell->gapStr, (char)key_char, CELL_TEXT_LENGTH);
            key_char = GetCharPressed(); // Check next character in the queue
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
            // if (*cellIndex > 0 && *cellIndex < 3 && *textChanged == true) {;;} // TODO: Update Player Names
            if (*cellIndex > 2 && *cellIndex < CELL_COUNT - 6) {
                    char *filteredText = filterText(cell->gapStr.str);
                    if (*textChanged == true) {
                        OverwriteStr(&cell->gapStr, filteredText, CELL_TEXT_LENGTH);
                    }
            *cellIndex = *cellIndex + 3;
            } else {
                *cellIndex = 0;
                selectionState = false;
            }
            if (*textChanged == true) UpdateScores(cellList);
            InputHandler(cell, cellIndex, selectionState, scoreTieAcc, textChanged);
        }
    }
}