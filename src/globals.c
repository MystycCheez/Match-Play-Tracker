#ifndef GLOBALS_C
#define GLOBALS_C

#include "headers.h"

Globals GVARS;
UI_Elements UI;
Window_Elements Window;
TitleBar_Elements TitleBar;
Cursor_Elements Cursor;
MouseState Mouse;
KeyboardState KeyData;
Cell* sheet;

Actions ActionTable[3][KEY_COMBO_COUNT];

// List of action functions
void (*Action[ACTION_COUNT])() = {
    A_DoNothing,
    A_ScopeDecrease,
    A_SelectChar,
    A_SelectAll,
    A_SelectAllAtCursorTowardsDir,
    A_MoveCursor,
    A_MoveCursorByToken,
    A_MoveCursorToStart,
    A_MoveCursorToEnd,
    A_LoadTimes,
    A_SaveTimes,
    A_ExportTimes,
    A_ClearTimes,
    A_Deselect_Or_Undo_Backout,
    A_Overwrite_UpdateScore,
    A_NavigateToNextCell,
    A_Copy,
    A_Cut,
    A_Paste,
    A_Delete,
    A_DeleteCellText,
    A_DeleteCellTextAndEnterInto,
    A_DeleteSelection,
    A_DeleteChar,
    A_NavigateLeft,
    A_NavigateRight,
    A_NavigateUp,
    A_NavigateDown,
    A_SwapVetoColor,
};

#endif