#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "includes.h"

void reInitGVARS()
{
    GVARS.screenHeight = GetScreenHeight();
    GVARS.screenWidth = GetScreenWidth();
    GVARS.cellHeight = GVARS.screenHeight / ROWS;
    GVARS.cellWidth = GVARS.screenWidth / COLUMNS;
    GVARS.fontSize = GVARS.screenHeight / 44;
}

void initButtons()
{
    GVARS.buttons = malloc(sizeof(Button) * 2);
    GVARS.buttons[BTN_EXIT].pos = (Vector2){GVARS.screenWidth - BUTTON_SIZE - (BUTTON_SIZE / 2), (TOP_BAR_HEIGHT / 2) - 13};
    GVARS.buttons[BTN_EXIT].texture = LoadTextureFromImage(LoadImage("resources/x.png"));
    GVARS.buttons[BTN_EXIT].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(GVARS.buttons[BTN_EXIT].texture, TEXTURE_FILTER_BILINEAR);
    GVARS.buttons[BTN_MINIMIZE].pos = (Vector2){GVARS.screenWidth - (BUTTON_SIZE * 3), (TOP_BAR_HEIGHT / 2) - 13};
    GVARS.buttons[BTN_MINIMIZE].texture = LoadTextureFromImage(LoadImage("resources/minimize.png"));
    GVARS.buttons[BTN_MINIMIZE].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(GVARS.buttons[BTN_MINIMIZE].texture, TEXTURE_FILTER_BILINEAR);
}

void initSheetText(Cell *sheet, Players players, int game)
{
    char **levelText = loadLevelText(game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&sheet[(i * 3) + 3].gapStr, levelText[i], CELL_TEXT_LENGTH);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%lld", players.s1);
    sprintf(s2, "%lld", players.s2);
    placeString(&sheet[0].gapStr, "Stage", CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 3].gapStr, "Points", CELL_TEXT_LENGTH);
    placeString(&sheet[1].gapStr, players.p1, CELL_TEXT_LENGTH);
    placeString(&sheet[2].gapStr, players.p2, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 2].gapStr, s1, CELL_TEXT_LENGTH);
    placeString(&sheet[CELL_COUNT - 1].gapStr, s2, CELL_TEXT_LENGTH);
}

Cell *initSheet(Players players, int game)
{
    Cell *sheet = malloc(sizeof(Cell) * CELL_COUNT);
    for (size_t i = 0; i < CELL_COUNT; i++) {
        sheet[i].gapStr = initGapStr(CELL_TEXT_LENGTH);
        sheet[i].alignment = ALIGN_CENTER;
        sheet[i].color = WHITE;
        sheet[i].highlight = TRANSPARENT;
        sheet[i].selectable = true;
    }
    sheet[0].selectable = false;
    sheet[CELL_COUNT - 1].selectable = false;
    sheet[CELL_COUNT - 2].selectable = false;
    sheet[CELL_COUNT - 3].selectable = false;
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        sheet[3 + (i * COLUMNS)].alignment = ALIGN_LEFT;
        sheet[3 + (i * COLUMNS)].color = COLOR_LEVEL;
        sheet[3 + (i * COLUMNS)].selectable = false;
    }
    initSheetText(sheet, players, game);
    return sheet;
}

void setBorderPositions(Line *borders)
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        borders[i] = (Line){0, (GVARS.cellHeight * i) + TOP_BAR_HEIGHT, GVARS.screenWidth, (GVARS.cellHeight * i) + TOP_BAR_HEIGHT};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        borders[i + index] = (Line){GVARS.cellWidth * i, TOP_BAR_HEIGHT, GVARS.cellWidth * i, GVARS.screenHeight};
    }
}

void ClearTimes(Cell* sheet)
{
    for (size_t i = 1; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2) || (i % 3 == 1)) {
            OverwriteStr(&sheet[i].gapStr, "", 0, CELL_TEXT_LENGTH);
            CellOverwriteHandler(sheet, i);
        }
    }
}

Color getStateColor(State_Button state)
{
    if (state == STATE_BTN_PRESSED)
        return GRAY;
    if (state == STATE_BTN_HIGHLIGHTED)
        return LIGHTGRAY;
    return WHITE;
}

Rectangle getButtonRect(Button button)
{
    return (Rectangle){button.pos.x, button.pos.y, BUTTON_SIZE, BUTTON_SIZE};
}

// Returns TEXT_VETO or TEXT_DNF if found
// Returns TEXT_NA if special text not found
Text_Type CompareSpecialText(char *text)
{
    Text_Type returnVal = TEXT_NA;
    for (size_t i = 0; i < GVARS.specials.count; i++) {
        if (strcmp(text, GVARS.specials.text[i]) == 0) {
            if (i < 4)
                returnVal = TEXT_VETO;
            if (i > 3)
                returnVal = TEXT_DNF;
        }
        else if (strcmp(text, GVARS.specials.text[i]) == 0)
            return returnVal;
    }
    return returnVal;
}

// TODO: check if this can utilize xy/cr functions
Vector2 CalcTextPos(Vector2 pos, size_t index)
{
    pos.x = pos.x + (GVARS.cellWidth * (index % 3));
    pos.y = 1 + pos.y + (GVARS.cellHeight * (index / 3) + TOP_BAR_HEIGHT);
    return pos;
}

// Expects format: "mm:ss"
size_t timeToSecs(char *time)
{
    size_t timeLen = strlen(time);
    assert(timeLen < 6 && "use filterCellText");
    if (timeLen < 3) {
        return atoi(time);
    }
    else {
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
    if (totalSecs == 0) return "\0";
    char *time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    size_t minutes = totalSecs / 60 < 100 ? totalSecs / 60 : 59;
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

// Filters string to be converted into time / Outputs "mm:ss" or "m:ss"
char *filterText(char *text)
{
    // Do these need to be static?
    static char *dummy = "\0";
    static char *text_veto = "Veto";
    static char *text_dnf = "DNF";

    char *filtered = malloc(sizeof(char) * 5);

    int special = CompareSpecialText(text);
    if (special == TEXT_VETO) return text_veto;
    if (special == TEXT_DNF) return text_dnf;

    size_t textLen = strlen(text);

    if (strpbrk(text, ":1234567890") == NULL)
        return dummy;

    size_t cCount = countChars(text, ':', textLen);
    if (cCount > 2)
        return dummy;
    switch (cCount) {
    case 0:
        switch (textLen) {
        case 1: // x
            sprintf(filtered, "0:0%s", text);
            break;
        case 2: // xx
            sprintf(filtered, "0:%s", text);
            break;
        case 3: // xxx
            sprintf(filtered, "%c:%s", text[0], text + 1);
            break;
        case 4: // xxxx
            if (text[0] != '0') {
                sprintf(filtered, "%c%c:%s", text[0], text[1], text + 2);
            }
            else
                sprintf(filtered, "%c:%s", text[1], text + 2);
            break;
        default:
            return dummy;
        }
        return secsToTime(timeToSecs(filtered));
    case 1:
        if (text[textLen - 3] != ':') return dummy;
        switch (textLen) {
        case 3: // :xx
            sprintf(filtered, "0%s", text);
            break;
        case 4: // x:xx
            sprintf(filtered, "%s", text);
            break;
        case 5: // xx:xx
            sprintf(filtered, "%s", text);
            break;
        default:
            return dummy;
        }
        return secsToTime(timeToSecs(filtered));
    case 2: // TODO:
        if (text[textLen - 6] != ':')
            return dummy;
        if (!(textLen <= 8 && textLen >= 6))
            return dummy;
        sprintf(filtered, "%s", text + textLen - 5);
        // TODO: Is this a memory leak?
        return filterText(filtered);
    default:
        return dummy;
    }
}

// Compares two cells in a row and returns a pair of ints representing who won or lost
// Also checks for special cases
Int2 CompareTimes(size_t row, Cell *cells)
{
    size_t cellL = crToIndex((Vector2){1, (float)row});
    size_t cellR = crToIndex((Vector2){2, (float)row});

    Text_Type specialL = CompareSpecialText(cells[cellL].gapStr.str);
    Text_Type specialR = CompareSpecialText(cells[cellR].gapStr.str);

    size_t timeL = -1;
    size_t timeR = -1;
    if (specialL == -1) timeL = timeToSecs(cells[cellL].gapStr.str);
    if (specialR == -1) timeR = timeToSecs(cells[cellR].gapStr.str);

    if (cells[cellL].gapStr.str[0] == 0 || cells[cellR].gapStr.str[0] == 0 || specialL == TEXT_VETO || specialR == TEXT_VETO) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1}; // Indicates Veto
    }
    if ((timeL == timeR) || (specialL == TEXT_DNF && specialR == TEXT_DNF)) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0}; // Indicates Tie
    }
    if ((timeL < timeR) || (specialR == TEXT_DNF)) { // Why doesn't this work????
        cells[cellL].highlight = COLOR_WIN;
        cells[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    if ((timeL > timeR) || (specialL == TEXT_DNF)) {
        cells[cellL].highlight = COLOR_LOSE;
        cells[cellR].highlight = COLOR_WIN;
        return (Int2){0, 1};
    }
    // TODO: player gets WR as time and is highlighted GOLD (unlikely, but would like this feature) - UNTIEDS too
    // fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
    fprintf(stderr, "Shouldn't have reached here!\n");
    exit(-1);
}

// Compares times in each row and calculates the scores, overwriting the text
void UpdateScores(Cell *cells)
{
    size_t tieCounter = 0;
    bool win_set = false;
    OverwriteStr(&cells[CELL_COUNT - 2].gapStr, "0", 0, CELL_TEXT_LENGTH);
    OverwriteStr(&cells[CELL_COUNT - 1].gapStr, "0", 0, CELL_TEXT_LENGTH);
    Int2 *wins = malloc(sizeof(Int2) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        wins[i] = CompareTimes(i + 1, cells);
        if (wins[i].a == 0 && wins[i].b == 0) {
            tieCounter++;
        }
        if (wins[i].a > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        }
        else if (wins[i].b > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        }
        else if (wins[i].a > 0 || wins[i].b > 0) {
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str)), 0, CELL_TEXT_LENGTH);
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str)), 0, CELL_TEXT_LENGTH);
        }
        if (!win_set) {
            if ((atoi(gapStrToStr(cells[CELL_COUNT - 1].gapStr, CELL_TEXT_LENGTH)) >= 9) || (atoi(gapStrToStr(cells[CELL_COUNT - 2].gapStr, CELL_TEXT_LENGTH)) >= 9)) {
                // printf("%s", gapStrToStr(cells[crToIndex((Vector2){0, i + 1})].gapStr, CELL_TEXT_LENGTH));
                GVARS.level_win = gapStrToStr(cells[crToIndex((Vector2){0, i + 1})].gapStr, CELL_TEXT_LENGTH);
                win_set = true;
            }
        }
    }
}

#endif