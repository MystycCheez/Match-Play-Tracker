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

void placeChar(GapBuffer *gapStr, char c, size_t bufLen)
{
    if (gapStr->cStart == gapStr->cEnd) return;
    gapStr->str[gapStr->cStart++] = c;
    // printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd);
}

void placeString(GapBuffer *gapStr, const char *str, size_t bufLen)
{
    for (size_t i = 0; i < strlen(str); i++) {
        placeChar(gapStr, str[i], bufLen);
    }
}

void OverwriteStr(GapBuffer *gapStr, const char *str, size_t len)
{
    if (gapStr->str[0] == 0) return;
    memset(gapStr->str, 0, len + 1);
    gapStr->cStart = 0;
    gapStr->cEnd = len - 1;
    placeString(gapStr, str, len);
    // printf("%s\n", str);
}

void deleteChar(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return;
    gapStr->str[--gapStr->cStart] = 0;
    // printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
}

void cursorLeft(GapBuffer *gapStr)
{
    if (gapStr->cStart == 0) return;
    chrswap(gapStr->str + gapStr->cStart - 1, gapStr->str + gapStr->cEnd);
    gapStr->cStart--;
    gapStr->cEnd--;
    // printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
}

void cursorRight(GapBuffer *gapStr, size_t bufLen)
{
    if (bufLen - 1 == gapStr->cEnd) return;
    chrswap(gapStr->str + gapStr->cStart, gapStr->str + gapStr->cEnd + 1);
    gapStr->cEnd++;
    gapStr->cStart++;
    // printf("%s|%s\n", gapStr->str, gapStr->str + gapStr->cEnd + 1);
}

// TODO
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

char* gapStrToStr(GapBuffer gapStr, size_t len)
{
    size_t lenR = strlen(gapStr.str + gapStr.cEnd + 1);
    if (lenR > 0) {
        char* str = malloc(sizeof(char) * len + 1);
        memset(str, 0, len + 1);
        strncpy(str, gapStr.str, gapStr.cStart);
        strncpy(str + strlen(str), gapStr.str + gapStr.cEnd + 1, lenR);
        return str;
    } else return gapStr.str;
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