#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include "structs.h"

GapBuffer initGapStr(size_t len);
size_t rawIndexToGapIndex(size_t index, GapBuffer gapStr);
size_t gapStrLen(GapBuffer gapStr);
bool placeChar(GapBuffer *gapStr, char c);
void placeString(GapBuffer *gapStr, const char *str, size_t len);
void OverwriteStr(GapBuffer *gapStr, const char *str, size_t start, size_t len);
GapBuffer strToGapStr(char* str, size_t cursor);
char* gapStrToStr(GapBuffer gapStr, size_t maxLen);
void deleteCharAtCursor(GapBuffer *gapStr);
bool cursorLeft(GapBuffer *gapStr);
bool cursorRight(GapBuffer *gapStr);
bool CursorMoveDir(GapBuffer *gapStr, bool dir);
bool SelectChar(GapBuffer *gapStr, bool dir);
void SelectToIndex(GapBuffer *gapStr, bool dir, size_t index);
void Deselect();
void MoveCursorToIndex(GapBuffer *gapStr, size_t index);
void DeleteSelection(GapBuffer *gapStr);
void replaceChar(GapBuffer *gapStr, char c);
void CopyText(GapBuffer gapStr);

#endif