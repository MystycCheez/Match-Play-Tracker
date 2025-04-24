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

void placeChar(GapBuffer *gapStr, char c)
{
    if (gapStr->cStart == gapStr->cEnd) return;
    gapStr->str[gapStr->cStart++] = c;
    #ifdef GAP_DEBUG
    printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd);
    #endif
}

void placeString(GapBuffer *gapStr, const char *str)
{
    for (size_t i = 0; i < strlen(str); i++) {
        placeChar(gapStr, str[i]);
    }
}

void OverwriteStr(GapBuffer *gapStr, const char *str, size_t len)
{
    if (gapStr->str[0] == 0) return;
    memset(gapStr->str, 0, len + 1);
    gapStr->cStart = 0;
    gapStr->cEnd = len - 1;
    placeString(gapStr, str);
    #ifdef GAP_DEBUG
    printf("%s\n", str);
    #endif
}

void deleteChar(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return;
    gapStr->str[--gapStr->cStart] = 0;
    #ifdef GAP_DEBUG
    printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
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
    printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
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
    printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
    #endif
    return true;
}

void selectChar(Selection *selection, GapBuffer *gapStr, bool dir)
{
    if (!selection->exists) {
        selection->start = gapStr->cStart;
        selection->end = gapStr->cStart;
        selection->exists = true;
    }
    if (dir == DIR_LEFT) {
        if (!cursorLeft(gapStr)) return;
        if (gapStr->cStart <= selection->start && gapStr->cStart < selection->start) {
            selection->start--;
        } else selection->end--;
    }
    if (dir == DIR_RIGHT) {
        if (!cursorRight(gapStr)) return;
        if (gapStr->cStart >= selection->start && gapStr->cStart > selection->end) {
            selection->end++;
        } else selection->start++;
    }
    if (selection->start == selection->end) selection->exists = false;
    // printf("selection: %lld, cStart: %lld\n", selection->start, gapStr->cStart);
}

void Deselect(Selection *selection) {
    selection->exists = false;
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

GapBuffer initGapStr(size_t len)
{
    GapBuffer gapStr = {0};
    gapStr.str = malloc(sizeof(char) * len + 1);
    memset(gapStr.str, 0, len + 1);
    gapStr.cStart = 0;
    gapStr.cEnd = len - 1;
    return gapStr;
}