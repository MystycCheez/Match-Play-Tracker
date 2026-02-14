#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>

#include "raylib.h"

#include "enums.h"
#include "structs.h"

void ClearTimes();
Text_Type CompareSpecialText(char *text);
size_t getOppositeCellIndex();
Cell getOppositeCell();
Vector2 GetCellPos(size_t index);
size_t timeToSecs(char *time);
char *secsToTime(size_t totalSecs);
size_t countChars(char *text, char c, size_t len);
void setCellTextColor(char* text);
char *filterCellText(char *text);
Int2 CompareTimes(size_t row);
void UpdateScores();
Vector2 indexToXY(size_t index);
Vector2 indexToCR(size_t index);
size_t xyToIndex(Vector2 xy);
size_t xToCursorIndex(float x_);
size_t crToIndex(Vector2 cr);
char* ColorToHexText(Color color);
Color HexToColor(char* text);
void chrswap(char* ptr1, char* ptr2);
char* i_toStr(int num);
void updateSheetIndex(size_t newIndex);
void unselectCells();
void CleanUp();
void SetVetoColor(Cell* cell);
char* strCreate(size_t len);

#endif