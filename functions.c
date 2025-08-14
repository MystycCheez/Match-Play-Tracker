#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "includes.h"

void ClearTimes()
{
    for (size_t i = 1; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2) || (i % 3 == 1)) {
            OverwriteStr(&sheet[i].gapStr, "\0", 0, CELL_TEXT_LENGTH);
            CellOverwriteHandler(sheet, i);
        }
    }
    OverwriteStr(&sheet[1].gapStr, GVARS.players.p1, 0, CELL_TEXT_LENGTH);
    OverwriteStr(&sheet[2].gapStr, GVARS.players.p2, 0, CELL_TEXT_LENGTH);
}

Color getStateColor(Button_State state)
{
    if (state == STATE_BTN_PRESSED)
        return GRAY;
    if (state == STATE_BTN_HIGHLIGHTED)
        return LIGHTGRAY;
    return WHITE;
}

Rectangle getButtonRect(Button button)
{
    return (Rectangle){button.pos.x, button.pos.y, button.size.x, button.size.y};
}

Rectangle getCellRect(size_t cellIndex)
{
    Vector2 pos = indexToXY(cellIndex);
    return (Rectangle){pos.x, pos.y, UI.cellWidth, UI.cellHeight};
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
    pos.x = pos.x + (UI.cellWidth * (index % 3));
    pos.y = 1 + pos.y + (UI.cellHeight * (index / 3) + TOP_BAR_HEIGHT);
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
char *filterCellText(char *text)
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

    Text_Type specialL = CompareSpecialText(sheet[cellL].gapStr.str);
    Text_Type specialR = CompareSpecialText(sheet[cellR].gapStr.str);

    size_t timeL = -1;
    size_t timeR = -1;
    if (specialL == -1) timeL = timeToSecs(sheet[cellL].gapStr.str);
    if (specialR == -1) timeR = timeToSecs(sheet[cellR].gapStr.str);

    if (sheet[cellL].gapStr.str[0] == 0 || sheet[cellR].gapStr.str[0] == 0 || specialL == TEXT_VETO || specialR == TEXT_VETO) {
        sheet[cellL].highlight = TRANSPARENT;
        sheet[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1}; // Indicates Veto
    }
    if ((timeL == timeR) || (specialL == TEXT_DNF && specialR == TEXT_DNF)) {
        sheet[cellL].highlight = TRANSPARENT;
        sheet[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0}; // Indicates Tie
    }
    if ((timeL < timeR) || (specialR == TEXT_DNF)) { // Why doesn't this work????
        sheet[cellL].highlight = COLOR_WIN;
        sheet[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    if ((timeL > timeR) || (specialL == TEXT_DNF)) {
        sheet[cellL].highlight = COLOR_LOSE;
        sheet[cellR].highlight = COLOR_WIN;
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
    OverwriteStr(&sheet[CELL_COUNT - 2].gapStr, "0", 0, CELL_TEXT_LENGTH);
    OverwriteStr(&sheet[CELL_COUNT - 1].gapStr, "0", 0, CELL_TEXT_LENGTH);
    Int2 *wins = malloc(sizeof(Int2) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        wins[i] = CompareTimes(i + 1);
        if (wins[i].a == 0 && wins[i].b == 0) {
            tieCounter++;
        }
        if (wins[i].a > 0 && tieCounter > 0) {
            OverwriteStr(&sheet[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(sheet[CELL_COUNT - 2].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        }
        else if (wins[i].b > 0 && tieCounter > 0) {
            OverwriteStr(&sheet[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(sheet[CELL_COUNT - 1].gapStr.str) + tieCounter), 0, CELL_TEXT_LENGTH);
            tieCounter = 0;
        }
        else if (wins[i].a > 0 || wins[i].b > 0) {
            OverwriteStr(&sheet[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(sheet[CELL_COUNT - 2].gapStr.str)), 0, CELL_TEXT_LENGTH);
            OverwriteStr(&sheet[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(sheet[CELL_COUNT - 1].gapStr.str)), 0, CELL_TEXT_LENGTH);
        }
        if (!win_set) {
            if ((atoi(gapStrToStr(sheet[CELL_COUNT - 1].gapStr, CELL_TEXT_LENGTH)) >= 9) || (atoi(gapStrToStr(sheet[CELL_COUNT - 2].gapStr, CELL_TEXT_LENGTH)) >= 9)) {
                GVARS.level_win = gapStrToStr(sheet[crToIndex((Vector2){0, i + 1})].gapStr, CELL_TEXT_LENGTH);
                win_set = true;
            }
        }
    }
}

bool getMoveDir()
{
    if (KeyData.pressed == KEY_LEFT) return DIR_LEFT;
    if (KeyData.pressed == KEY_RIGHT) return DIR_RIGHT;
    assert(!"Shouldn't be trying to get a direction when not pressing left or right!");
}

int GetKeyComboIndex(int key, int modifier)
{
    if (key == -1) return UNUSED_KEY_COMBO;
    return key + (modifier * KEY_COUNT);
}

char* GetKeyComboText(int index)
{
    size_t size = 50;
    size_t pos = 0;
    char* buffer = calloc(size, 1);
    if (((index / KEY_COUNT) >= 1) && ((index / KEY_COUNT) < 2)) {
        pos += snprintf(buffer, size - pos, "Ctrl + ");
    }
    if (((index / KEY_COUNT) >= 2) && ((index / KEY_COUNT) < 3)) {
        pos += snprintf(buffer + pos, size - pos, "Shift + ");
    }
    if ((index / KEY_COUNT) >= 3) {
        pos += snprintf(buffer + pos, size - pos, "Ctrl + Shift + ");
    }
    index = (index % KEY_COUNT);
    const char* tmp = GetKeyName(index);
    snprintf(buffer + pos, pos - size, tmp);
    return buffer;
}

const char* GetKeyName(int index)
{
    if (index == -1) return "N/A";
    return keynames[index];
}

const char* GetActionText(int index)
{
    return actionnames[index];
}

const char* GetHumanReadableActionText(int index)
{
    return actionnames_humanreadable[index];
}

int GetModifier()
{
    int modifier = 0;
    if (KeyData.ctrl) modifier += 1;
    if (KeyData.shift) modifier += 2;
    if (KeyData.alt) modifier += 3;
    return modifier;
}

int GetKeyIndex(int key)
{
    for (size_t i = 0; i < KEY_COUNT; i++) {
        if (key == Raylib_Keys[i]) return i;
    }
    return -1;
}

#endif