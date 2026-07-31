#ifndef GAP_BUFFER_C
#define GAP_BUFFER_C

// #define GAP_DEBUG

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "functions.h"
#include "globals.h"
#include "gap_buffer.h"

GapBuffer InitGapStr(size_t len)
{
    GapBuffer gapStr;

    gapStr.str = malloc(sizeof(char) * len);
    memset(gapStr.str, 0, len);

    gapStr.gapStart = 0;
    gapStr.gapEnd = 0;
    return gapStr;
}

size_t gapStrLen(GapBuffer gapStr)
{
    return gapStr.gapEnd - gapStr.gapStart;
}

// Returns false if no room for another char
bool placeChar(GapBuffer* gapStr, char c)
{
    if (gapStr->gapStart == gapStr->gapEnd) return false;
    gapStr->str[gapStr->gapStart++] = c;
    return true;
}

void placeString(GapBuffer* gapStr, const char* str)
{
    size_t index = 0;
    while (index < gapStr->len) {
        if (placeChar(gapStr, str[index++]) == false) return;
    }
}

void OverwriteStr(GapBuffer *gapStr, const char *str, size_t start)
{
    memset(gapStr->str, 0, gapStr->len);
    gapStr->gapStart = start;
    gapStr->gapEnd = gapStr->len;
    placeString(gapStr, str);
}

char* gapStrToStr(GapBuffer gapStr)
{
    char* str = malloc(sizeof(char) * gapStr.len + 1);
    memset(str, 0, gapStr.len + 1);
    
    size_t sIndex = 0;
    size_t gIndex = 0;
    while (sIndex < gapStr.len) {
        if (gIndex == gapStr.gapStart) gIndex = gapStr.gapEnd;
        str[sIndex++] = gapStr.str[gIndex++];
    }
    return str;
}

void deleteCharAtCursor(GapBuffer *gapStr)
{
    if (gapStr->gapStart == 0) return;
    gapStr->gapStart--;
}

// Returns false if no movement occured
bool cursorLeft(GapBuffer *gapStr)
{
    if (gapStr->gapStart == 0) return false;
    chrswap(gapStr->str + gapStr->gapStart - 1, gapStr->str + gapStr->gapEnd);
    gapStr->gapStart--;
    gapStr->gapEnd--;
    return true;
}

// Returns false if no movement occured
bool cursorRight(GapBuffer *gapStr)
{
    if (gapStr->gapEnd == CELL_TEXT_LENGTH - 1) return false;
    chrswap(gapStr->str + gapStr->gapStart, gapStr->str + gapStr->gapEnd + 1);
    gapStr->gapEnd++;
    gapStr->gapStart++;
    return true;
}

bool CursorMoveDir(GapBuffer *gapStr, bool dir)
{
    if (dir == DIR_LEFT) return cursorLeft(gapStr);
    if (dir == DIR_RIGHT) return cursorRight(gapStr);
    assert(!"Move Direction is not valid!");
}

bool SelectLeft(GapBuffer* gapStr)
{
    if (!cursorLeft(gapStr)) {
        return false;
    } else if (gapStr->gapStart < Sheet.selection.start) {
        Sheet.selection.start--;
    } else Sheet.selection.len--;
}

bool SelectRight(GapBuffer* gapStr)
{
    if (!cursorRight(gapStr)) {
        return false;
    } else if (gapStr->gapStart > Sheet.selection.start + Sheet.selection.len) {
        Sheet.selection.len++;
    } else Sheet.selection.start++;
}

bool SelectMoveDir(GapBuffer *gapStr, bool dir)
{
    if (dir == DIR_LEFT) return SelectLeft(gapStr);
    if (dir == DIR_RIGHT) return SelectRight(gapStr);
    assert(!"Move Direction is not valid!");
    return true;
}

void SelectToIndex(GapBuffer *gapStr, bool dir, size_t index)
{
    while (Sheet.selection.len != index || Sheet.selection.start != index) {
        SelectChar(gapStr, dir);
    }
}

void MoveCursorToIndex(GapBuffer *gapStr, size_t index)
{
    while (gapStr->gapStart > index) {
        cursorLeft(gapStr);
        if (!(gapStr->gapStart > index)) return;
    } 
    while (gapStr->gapStart < index) {
        cursorRight(gapStr);
    }
}

void DeleteSelection(GapBuffer *gapStr)
{
    if (!Sheet.selection.len) return;
    MoveCursorToIndex(gapStr, Sheet.selection.len);
    while (gapStr->gapStart > Sheet.selection.start) {
        deleteCharAtCursor(gapStr);
    }
}

void replaceChar(GapBuffer *gapStr, char c)
{
    char* tmp = gapStrToStr(*gapStr);
    char* tmp2 = strCreate(CELL_TEXT_LENGTH); 
    tmp2 = strcpy(tmp2, &tmp[Sheet.selection.start]);
    free(tmp);
    tmp2[0] = c;
    OverwriteStr(gapStr, tmp2, 0);
}

void CopyText(GapBuffer gapStr)
{
    char *copy = strCreate(CELL_TEXT_LENGTH);
    char *tmp = gapStrToStr(gapStr);
    memset(copy, 0, CELL_TEXT_LENGTH);
    strncpy(copy, tmp + Sheet.selection.start, Sheet.selection.len - Sheet.selection.start);
    SetClipboardText(copy);
    free(tmp);
    free(copy);
}

void Deselect()
{
    Sheet.selection.len = 0;
}

#endif