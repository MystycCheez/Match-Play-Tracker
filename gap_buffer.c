#include "includes.h"

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

size_t rawIndexToGapIndex(size_t index, GapBuffer gapStr)
{
    size_t a = 0;
    size_t b = 0;
    while (gapStr.str[a] != 0) {
        if (a == index) return a;
        a++;
    }
    while (gapStr.str[gapStr.cEnd - 1 + b] != 0) {
        if (b == index) return a + b;
        b++;
    }
    fprintf(stderr, "ERROR: Provided index longer than gapStrLen!\n");
    exit(1);
}

size_t gapStrLen(GapBuffer gapStr)
{
    size_t a = 0;
    size_t b = 0;
    while (gapStr.str[a++] != 0) {}
    while (gapStr.str[gapStr.cEnd - 1 + b++] != 0) {}
    return a + b;
}

void placeChar(GapBuffer *gapStr, char c)
{
    if (gapStr->cStart == gapStr->cEnd) return;
    gapStr->str[gapStr->cStart++] = c;
    #ifdef GAP_DEBUG
    printf("placeChar: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd);
    #endif
}

void placeString(GapBuffer *gapStr, const char *str, size_t len)
{
    for (size_t i = 0; i < strlen(str) && i < len; i++) {
        placeChar(gapStr, str[i]);
    }
    #ifdef GAP_DEBUB
    printf("placeString: %s\n", gapStr->str);
    #endif
}

void OverwriteStr(GapBuffer *gapStr, const char *str, size_t start, size_t len)
{
    memset(gapStr->str + start, 0, len + 1);
    gapStr->cStart = start;
    gapStr->cEnd = len - 1;
    placeString(gapStr, str, len);
    #ifdef GAP_DEBUG
    printf("OverwriteStr: %s\n", gapStr->str);
    #endif
}

void deleteChar(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return;
    gapStr->str[--gapStr->cStart] = 0;
    #ifdef GAP_DEBUG
    printf("deleteChar: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
    #endif
}

// Returns false if no movement occured
bool cursorLeft(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return false;
    chrswap(gapStr->str + gapStr->cStart - 1, gapStr->str + gapStr->cEnd);
    gapStr->cStart--;
    gapStr->cEnd--;
    #ifdef GAP_DEBUG
    printf("cursorLeft: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
    #endif
    return true;
}

// Returns false if no movement occured
bool cursorRight(GapBuffer *gapStr)
{
    if (gapStr->cEnd == CELL_TEXT_LENGTH - 1) return false;
    chrswap(gapStr->str + gapStr->cStart, gapStr->str + gapStr->cEnd + 1);
    gapStr->cEnd++;
    gapStr->cStart++;
    #ifdef GAP_DEBUG
    printf("cursorRight: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
    #endif
    return true;
}

void selectChar(GapBuffer *gapStr, bool dir)
{
    if (!GVARS.selection.exists) {
        GVARS.selection.start = gapStr->cStart;
        GVARS.selection.end = gapStr->cStart;
        GVARS.selection.exists = true;
    }
    if (dir == DIR_LEFT) {
        if (!cursorLeft(gapStr)) {
            if (GVARS.selection.start == GVARS.selection.end) {GVARS.selection.exists = false;}
            return;
        }
        if (gapStr->cStart <= GVARS.selection.start && gapStr->cStart < GVARS.selection.start) {
            GVARS.selection.start--;
        } else GVARS.selection.end--;
    }
    if (dir == DIR_RIGHT) {
        if (!cursorRight(gapStr)) {
            if (GVARS.selection.start == GVARS.selection.end) {GVARS.selection.exists = false;}
            return;
        }
        if (gapStr->cStart >= GVARS.selection.start && gapStr->cStart > GVARS.selection.end) {
            GVARS.selection.end++;
        } else GVARS.selection.start++;
    }
    if (GVARS.selection.start == GVARS.selection.end) GVARS.selection.exists = false;
    // printf("start: %lld, end: %lld\n", GVARS.selection.start, GVARS.selection.end);
}

void Deselect() {
    GVARS.selection.exists = false;
    // Do I need to adjust the start/end?
}

void GapStrGotoIndex(GapBuffer *gapStr, size_t index)
{
    while (gapStr->cStart > index) {
        cursorLeft(gapStr);
        if (!(gapStr->cStart > index)) return;
    } 
    while (gapStr->cStart < index) {
        cursorRight(gapStr);
    }
}

// TODO // TODO What?
GapBuffer strToGapStr(char* str, size_t cursor)
{
    GapBuffer gapStr = {0};
    size_t len = strlen(str);
    gapStr.str = malloc(sizeof(char) * len);
    memset(gapStr.str, 0, len);
    snprintf(gapStr.str, cursor, str);
    gapStr.cStart = 0;
    gapStr.cEnd = cursor;
    return gapStr;
}

char* gapStrToStr(GapBuffer gapStr, size_t maxLen)
{
    if (maxLen == 0) return "\0";
    size_t initLenL = strlen(gapStr.str);
    size_t initLenR = strlen(gapStr.str + gapStr.cEnd + 1);
    size_t len = min(maxLen, initLenL + initLenR);
    char* str = malloc(sizeof(char) * len + 1);
    memset(str, 0, len + 1);
    size_t lenL = min(len, initLenL);
    strncpy(str, gapStr.str, lenL);
    if (initLenR > 0 && len > initLenL) {
        size_t lenR = len - lenL;
        strncpy(str + lenL, gapStr.str + gapStr.cEnd + 1, lenR);
    }
    return str;
}

void replaceChar(GapBuffer *gapStr, char c)
{
    char* tmp = gapStrToStr(*gapStr, CELL_TEXT_LENGTH);
    char* tmp2 = malloc(sizeof(char) * CELL_TEXT_LENGTH); 
    tmp2 = strcpy(tmp2, &tmp[GVARS.selection.start]);
    free(tmp);
    tmp2[0] = c;
    OverwriteStr(gapStr, tmp2, 0, CELL_TEXT_LENGTH);
}

GapBuffer initGapStr(size_t len)
{
    GapBuffer gapStr = {0};
    gapStr.str = malloc(sizeof(char) * len + 1);
    memset(gapStr.str, 0, len + 1);
    gapStr.cStart = 0;
    gapStr.cEnd = len - 1;
    return gapStr;
}