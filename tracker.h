#ifndef TRACKER_H
#define TRACKER_H

#include "defines.h"
#include "enums.h"
#include "structs.h"

void reInitGVARS();
void initButtons();
void initSheetText(Cell *sheet, Players players, int game);
Cell *initSheet(Players players, int game);
void setBorderPositions(Line *borders);
void ClearTimes(Cell* sheet);
Color getStateColor(State_Button state);
Rectangle getButtonRect(Button button);
Text_Type CompareSpecialText(char *text);
Vector2 CalcTextPos(Vector2 pos, size_t index);
size_t timeToSecs(char *time);
char *secsToTime(size_t totalSecs);
size_t countChars(char *text, char c, size_t len);
char *filterText(char *text);
Int2 CompareTimes(size_t row, Cell *cells);
void UpdateScores(Cell *cells);

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
void selectChar(GapBuffer *gapStr, bool dir);
void Deselect();
void MoveCursorToIndex(GapBuffer *gapStr, size_t index);
void DeleteSelection(GapBuffer *gapStr);
void replaceChar(GapBuffer *gapStr, char c);
void CopyText(GapBuffer gapStr);

Vector2 indexToXY(size_t index);
Vector2 indexToCR(size_t index);
size_t xyToIndex(Vector2 xy);
size_t crToIndex(Vector2 cr);
char* ColorToHexText(Color color);
void chrswap(char* ptr1, char* ptr2);
char* i_toStr(int num);

Font loadFont();
void loadTimes(Cell* sheet);
char **loadLevelText(int game);
void loadSpecialText();
void saveTimes(Cell* sheet);
void ExportToBBCode(Cell* sheet);

void DrawCellBorders(size_t cellIndex);
void DrawCursor(Cell *sheet, size_t cellIndex, Font font);
void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell);
void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell);
void DrawTextAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell, size_t i);
void DrawTextHighlight(Cell *sheet, size_t cellIndex, Font font);

void ExitHandler();
void MouseTitleBarHandler(CollisionMap Collision, MouseState Mouse, Vector2 mousePos, Vector2 windowPos);
void MouseSheetHandler(CollisionMap Collision, MouseState Mouse, Vector2 mousePos, Cell *sheet, size_t *cellIndex);
void MouseHandler(Cell *sheet, size_t *cellIndex);
void EnterNavigationHandler(Cell *sheet, size_t *cellIndex);
void CellOverwriteHandler(Cell *sheet, size_t *cellIndex);
void CellInputHandler(Cell *sheet, size_t *cellIndex);
void SheetKeyPressHandler(Cell *sheet, KeyMap key, size_t *cellIndex);
void CellKeyPressHandler(Cell *sheet, KeyMap key, size_t *cellIndex);
void InputHandler(Cell *sheet, size_t *cellIndex);

#endif