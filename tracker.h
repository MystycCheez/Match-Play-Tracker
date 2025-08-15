#ifndef TRACKER_H
#define TRACKER_H

#include "defines.h"
#include "enums.h"
#include "structs.h"

// init.c
void initGlobals();
void initWindow();
void initButtons();
void initSheetText();
Cell *initSheet();
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
Vector2 CalcTextPos(Vector2 pos, size_t index);
size_t timeToSecs(char *time);
char *secsToTime(size_t totalSecs);
size_t countChars(char *text, char c, size_t len);
char *filterCellText(char *text);
Int2 CompareTimes(size_t row);
void UpdateScores();
Vector2 indexToXY(size_t index);
Vector2 indexToCR(size_t index);
size_t xyToIndex(Vector2 xy);
size_t crToIndex(Vector2 cr);
char* ColorToHexText(Color color);
void chrswap(char* ptr1, char* ptr2);
char* i_toStr(int num);

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
void selectChar(GapBuffer *gapStr, bool dir);
void Deselect();
void MoveCursorToIndex(GapBuffer *gapStr, size_t index);
void DeleteSelection(GapBuffer *gapStr);
void replaceChar(GapBuffer *gapStr, char c);
void CopyText(GapBuffer gapStr);

// io.c
void loadFont();
bool loadTimes();
char **loadLevelText();
void loadSpecialText();
void saveTimes();
void ExportToBBCode();

// draw.c
void DrawSelectionBorders();
void DrawCursor();
void DrawTextCentered();
void DrawTextLeftAligned();
void DrawTextAligned();
void DrawTextHighlight();

// handler.c
void ExitHandler();
void MouseTitleBarHandler(CollisionMap Collision, Vector2 windowPos);
void MouseSheetHandler(CollisionMap Collision);
void MouseHandler();
void EnterNavigationHandler();
void CellOverwriteHandler();
void CellInputHandler();
void InputHandler();
void CursorHandler();

// actions.c
void A_DoNothing();
void A_ScopeDecrease();
void A_SelectChar();
void A_MoveCursor();
void A_MoveCursorByToken();
void A_MoveCursorToStart();
void A_MoveCursorToEnd();
void A_LoadTimes();
void A_SaveTimes();
void A_ExportTimes();
void A_ClearTimes();
void A_Deselect_Or_Undo_Backout();
void A_Overwrite_UpdateScore();
void A_NavigateToNextCell();
void A_Copy();
void A_Cut();
void A_Paste();
void A_Delete();
void A_DeleteCellText();
void A_DeleteCellTextAndEnterInto();
void A_DeleteSelection();
void A_DeleteChar();
void A_NavigateLeft();
void A_NavigateRight();
void A_NavigateUp();
void A_NavigateDown();
void A_SwapVetoColor();

#endif