#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "headers.h"

void ClearTimes()
{
    for (size_t i = 1; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2) || (i % 3 == 1)) {
            OverwriteStr(&Sheet.cellList[i].gapStr, "\0", 0, CELL_TEXT_LENGTH);
            CellOverwriteHandler(Sheet.cellList, i);
        }
    }
    OverwriteStr(&Sheet.cellList[1].gapStr, Sheet.players.p1, 0, CELL_TEXT_LENGTH);
    OverwriteStr(&Sheet.cellList[2].gapStr, Sheet.players.p2, 0, CELL_TEXT_LENGTH);
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

// Gets the index of the opposite cell (same row) of the current cell index
size_t getOppositeCellIndex()
{
    if (Sheet.index % 3 == 1) {
        return Sheet.index + 1;
    } else return Sheet.index - 1;
}

// Gets the opposite cell (same row) of the current cell
Cell getOppositeCell()
{
    return Sheet.cellList[getOppositeCellIndex()];
}

// TODO: check if this can utilize xy/cr functions
Vector2 CalcTextPos(Vector2 pos, size_t index)
{
    pos.x = pos.x + (UI.cellWidth * (index % 3));
    pos.y = 1 + pos.y + (UI.cellHeight * (index / 3) + UI.topBarHeight);
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

void setCellTextColor(char* text)
{
    Sheet.cell->color = HexToColor(text);
}

// Filters string to be converted into time / Outputs "mm:ss" or "m:ss"
char *filterCellText(char* text)
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
    case 2: // TODO: two colons (xx:xx:xx)
        if (text[textLen - 6] != ':')
            return dummy;
        if (!(textLen <= 8 && textLen >= 6))
            return dummy;
        sprintf(filtered, "%s", text + textLen - 5);
        // TODO: Is this a memory leak?
        return filterCellText(filtered);
    default:
        return dummy;
    }
}

// Compares two cells in a row and returns a pair of ints representing who won or lost
// Also checks for special cases
Int2 CompareTimes(size_t row)
{
    size_t cellL = crToIndex((Vector2){1, (float)row});
    size_t cellR = crToIndex((Vector2){2, (float)row});

    Text_Type specialL = CompareSpecialText(Sheet.cellList[cellL].gapStr.str);
    Text_Type specialR = CompareSpecialText(Sheet.cellList[cellR].gapStr.str);

    size_t timeL = -1;
    size_t timeR = -1;
    if (specialL == -1) timeL = timeToSecs(Sheet.cellList[cellL].gapStr.str);
    if (specialR == -1) timeR = timeToSecs(Sheet.cellList[cellR].gapStr.str);

    if (Sheet.cellList[cellL].gapStr.str[0] == 0 || Sheet.cellList[cellR].gapStr.str[0] == 0 || specialL == TEXT_VETO || specialR == TEXT_VETO) {
        Sheet.cellList[cellL].highlight = TRANSPARENT;
        Sheet.cellList[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1}; // Indicates Veto
    }
    if ((timeL == timeR) || (specialL == TEXT_DNF && specialR == TEXT_DNF)) {
        Sheet.cellList[cellL].highlight = TRANSPARENT;
        Sheet.cellList[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0}; // Indicates Tie
    }
    if ((timeL < timeR) || (specialR == TEXT_DNF)) { // Why doesn't this work????
        Sheet.cellList[cellL].highlight = COLOR_WIN;
        Sheet.cellList[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    if ((timeL > timeR) || (specialL == TEXT_DNF)) {
        Sheet.cellList[cellL].highlight = COLOR_LOSE;
        Sheet.cellList[cellR].highlight = COLOR_WIN;
        return (Int2){0, 1};
    }
    // TODO: player gets WR as time and is highlighted GOLD (unlikely, but would like this feature) - UNTIEDS too
    // fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
    fprintf(stderr, "Shouldn't have reached here!\n");
    exit(-1);
}

// Compares times in each row and calculates the scores, overwriting the text
void UpdateScores()
{
    size_t tieCounter = 0;
    bool win_set = false;
    OverwriteStr(&Sheet.cellList[CELL_COUNT - 2].gapStr, "0", 0, CELL_TEXT_LENGTH);
    OverwriteStr(&Sheet.cellList[CELL_COUNT - 1].gapStr, "0", 0, CELL_TEXT_LENGTH);
    Int2 *wins = malloc(sizeof(Int2) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        wins[i] = CompareTimes(i + 1);
        if (wins[i].a == 0 && wins[i].b == 0) {
            tieCounter++;
        }
        if (wins[i].a > 0 && tieCounter > 0) {
            OverwriteStr(&Sheet.cellList[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(Sheet.cellList[CELL_COUNT - 2].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].b > 0 && tieCounter > 0) {
            OverwriteStr(&Sheet.cellList[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(Sheet.cellList[CELL_COUNT - 1].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].a > 0 || wins[i].b > 0) {
            OverwriteStr(&Sheet.cellList[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(Sheet.cellList[CELL_COUNT - 2].gapStr.str)), 0, CELL_TEXT_LENGTH);
            OverwriteStr(&Sheet.cellList[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(Sheet.cellList[CELL_COUNT - 1].gapStr.str)), 0, CELL_TEXT_LENGTH);
        }
        if (!win_set) {
            if ((atoi(gapStrToStr(Sheet.cellList[CELL_COUNT - 1].gapStr, CELL_TEXT_LENGTH)) >= 9) || (atoi(gapStrToStr(Sheet.cellList[CELL_COUNT - 2].gapStr, CELL_TEXT_LENGTH)) >= 9)) {
                Sheet.level_win = gapStrToStr(Sheet.cellList[crToIndex((Vector2){0, i + 1})].gapStr, CELL_TEXT_LENGTH);
                win_set = true;
            }
        }
    }
}

// XY being the top left corner of the cell index
Vector2 indexToXY(size_t index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * UI.cellWidth;
    xy.y = (index / COLUMNS) * UI.cellHeight + UI.topBarHeight;
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
    xy.y = xy.y - UI.topBarHeight;
    Vector2 rounded = {xy.x - fmodf(xy.x, UI.cellWidth), xy.y - fmodf(xy.y, UI.cellHeight)};
    rounded.x = rounded.x / UI.cellWidth;
    rounded.y = rounded.y / UI.cellHeight;
    index = rounded.x + (rounded.y * COLUMNS);
    return index;
}

// CR being column/row
size_t crToIndex(Vector2 cr)
{
    return (size_t)(cr.x + (cr.y * COLUMNS));
}

char* ColorToHexText(Color color)
{
    char* colorText = malloc(8);
    snprintf(colorText + 0, 2, "#");
    snprintf(colorText + 1, 3, "%02x", color.r);
    snprintf(colorText + 3, 3, "%02x", color.g);
    snprintf(colorText + 5, 3, "%02x", color.b);
    return colorText;
}

Color HexToColor(char* text)
{
    char* r = malloc(2);
    char* g = malloc(2);
    char* b = malloc(2);
    strncpy(r, text + 1, 2);
    strncpy(g, text + 3, 2);
    strncpy(b, text + 5, 2);
    int R = strtol(r, NULL, 16);
    int G = strtol(g, NULL, 16);
    int B = strtol(b, NULL, 16);
    Color color = {R, G, B, 0xFF};
    return color;
}

void chrswap(char* ptr1, char* ptr2)
{
    char tmp = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = tmp;
}

char* i_toStr(int num)
{
    // TODO: make more portable or just use itoa in place of this
    char* str = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(str, "%d", num);
    return str;
}

void updateSheetIndex(size_t newIndex)
{
    Sheet.index = newIndex;
    Sheet.cell = &Sheet.cellList[Sheet.index];
}

void CleanUp()
{
    UnloadFont(UI.font);
    UnloadTexture(TitleBar.Texture);
    UnloadTexture(Window.IconTexture);
    UnloadTexture(UI.buttons[BTN_EXIT].texture);
    UnloadTexture(UI.buttons[BTN_MINIMIZE].texture);
    for (size_t i = 0; i < 2; i++) {
        UnloadTexture(UI.buttons[i].texture);
    }   
}

void unselectCells()
{
    GVARS.scope = SCOPE_OVERVIEW;
    updateSheetIndex(0);
}

#endif