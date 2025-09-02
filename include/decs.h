#ifndef DECS_H
#define DECS_H

#include "headers.h"

void* debug_malloc(size_t size, const char *file, int line, const char *func);
void debug_free(void* p, const char *file, int line, const char *func);

// init.c
void initLinkedList(void* data);
void initGlobals();
void initWindow();
void initButtons();
void setGameText();
void initSheetText();
void initSheet();
void setBorderPositions();
void initActionTable();

// get.c
Color getStateColor(Button_State state);
Rectangle getButtonRect(Button button);
Rectangle getCellRect(size_t cellIndex);
bool getMoveDir();
int GetKeyComboIndex(int key, int modifier);
char* GetKeyComboText(int index);
int GetModifier();
int GetKeyIndex(int key);
const char* GetKeyText(int key);
int GetRaylibKey(size_t index);
const char* GetModifierText(Modifier modifier);
const char* GetActionText(int index);
const char* GetHumanReadableActionText(int index);

// functions.c
void ClearTimes();
Text_Type CompareSpecialText(char *text);
size_t getOppositeCellIndex();
Cell getOppositeCell();
Vector2 CalcTextPos(Vector2 pos, size_t index);
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
size_t crToIndex(Vector2 cr);
char* ColorToHexText(Color color);
Color HexToColor(char* text);
void chrswap(char* ptr1, char* ptr2);
char* i_toStr(int num);
void updateSheetIndex(size_t newIndex);
void CleanUp();
void unselectCells();

// gap_buffer.c
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
bool selectChar(GapBuffer *gapStr, bool dir);
void Deselect();
void MoveCursorToIndex(GapBuffer *gapStr, size_t index);
void DeleteSelection(GapBuffer *gapStr);
void replaceChar(GapBuffer *gapStr, char c);
void CopyText(GapBuffer gapStr);

// io.c
void loadFont();
bool loadTimes();
char **loadLevelText(int game);
void loadSpecialText();
void saveTimes();
void ExportToBBCode();
void ExportActionTable();

// draw.c
void DrawSelectionBorders();
void DrawCursor();
void DrawTextCentered(Vector2 pos, Cell cell);
void DrawTextLeftAligned(Vector2 pos, Cell cell);
void DrawTextAligned(Vector2 pos, Cell cell, size_t cellIndex);
void DrawTextHighlight();

// handler.c
void ExitHandler();
void MouseTitleBarHandler(CollisionMap Collision, Vector2 windowPos);
void MouseSheetHandler(CollisionMap Collision);
void MouseHandler();
void EnterNavigationHandler();
void CellOverwriteHandler();
void CellInputHandler();
void KeyHandler();
void CursorHandler();

// linked_list.c
Node* NewNode(void* data, Node* next, Node* prev);

#define ACTION(a,b,c) void b();
    ACTION_LIST
#undef ACTION

#endif