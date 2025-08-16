#ifndef GAP_BUFFER_C
#define GAP_BUFFER_C

#include "headers.h"

GapBuffer initGapStr(size_t len)
{
    GapBuffer gapStr = {0};
    gapStr.str = malloc(sizeof(char) * len + 1);
    memset(gapStr.str, 0, len + 1);
    gapStr.cStart = 0;
    gapStr.cEnd = len - 1;
    return gapStr;
}

// Does this code work?
size_t rawIndexToGapIndex(size_t index, GapBuffer gapStr)
{
    size_t a = 0;
    size_t b = 0;
    while (gapStr.str[a] != 0) {
        if (a == index) return a;
        a++;
    }
    while (gapStr.str[gapStr.cEnd - 1 + b] != 0) {
        // is there any reason why this shouldn't just be:
        // return b;
        // ???
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

// Returns false if no room for another char
bool placeChar(GapBuffer *gapStr, char c)
{
    if (gapStr->cStart == gapStr->cEnd) return false;
    gapStr->str[gapStr->cStart++] = c;
    #ifdef GAP_DEBUG
    printf("placeChar: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd);
    #endif
    return true;
}

void placeString(GapBuffer *gapStr, const char *str, size_t len)
{
    for (size_t i = 0; i < strlen(str) && i < len; i++) {
        if (placeChar(gapStr, str[i]) == false) return;
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

// I'm not using this function, do I need it?
// If so, check if it works
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

void deleteCharAtCursor(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return;
    gapStr->str[--gapStr->cStart] = 0;
    #ifdef GAP_DEBUG
    printf("deleteCharAtCursor: %s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
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

bool CursorMoveDir(GapBuffer *gapStr, bool dir)
{
    if (dir == DIR_LEFT) return cursorLeft(gapStr);
    if (dir == DIR_RIGHT) return cursorRight(gapStr);
    assert(!"Move Direction is not valid!");
}

// boy this is another mess
// return value indicates if movement occurred or not
bool selectChar(GapBuffer *gapStr, bool dir)
{
    if (!GVARS.selection.exists) {
        GVARS.selection.start = gapStr->cStart;
        GVARS.selection.end = gapStr->cStart;
        GVARS.selection.exists = true;
    }
    
    if (dir == DIR_LEFT) {
        if (!cursorLeft(gapStr)) {
            if (GVARS.selection.start == GVARS.selection.end) {GVARS.selection.exists = false;}
            return false;
        } else if (gapStr->cStart <= GVARS.selection.start && gapStr->cStart < GVARS.selection.start) {
            GVARS.selection.start--;
        } else GVARS.selection.end--;
    } else if (dir == DIR_RIGHT) {
        if (!cursorRight(gapStr)) {
            if (GVARS.selection.start == GVARS.selection.end) {GVARS.selection.exists = false;}
            return false;
        } else if (gapStr->cStart >= GVARS.selection.start && gapStr->cStart > GVARS.selection.end) {
            GVARS.selection.end++;
        } else GVARS.selection.start++;
    } else if (GVARS.selection.start == GVARS.selection.end) GVARS.selection.exists = false;
    if (GVARS.selection.start == GVARS.selection.end) {GVARS.selection.exists = false;}
    // printf("start: %lld, end: %lld\n", GVARS.selection.start, GVARS.selection.end);
    return true;
}

// This function exists because it might need to do more in the future
void Deselect()
{
    GVARS.selection.exists = false;
    // Do I need to adjust the start/end?
}

void MoveCursorToIndex(GapBuffer *gapStr, size_t index)
{
    while (gapStr->cStart > index) {
        cursorLeft(gapStr);
        if (!(gapStr->cStart > index)) return;
    } 
    while (gapStr->cStart < index) {
        cursorRight(gapStr);
    }
}

void DeleteSelection(GapBuffer *gapStr)
{
    MoveCursorToIndex(gapStr, GVARS.selection.end);
    while (gapStr->cStart > GVARS.selection.start) {
        deleteCharAtCursor(gapStr);
    }
    Deselect();
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

void CopyText(GapBuffer gapStr)
{
    char *copy = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *tmp = gapStrToStr(gapStr, CELL_TEXT_LENGTH);
    memset(copy, 0, CELL_TEXT_LENGTH);
    strncpy(copy, tmp + GVARS.selection.start, GVARS.selection.end);
    SetClipboardText(copy);
    free(tmp);
    free(copy);
}

#endif