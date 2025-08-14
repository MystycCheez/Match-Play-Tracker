#ifndef ACTIONS_C
#define ACTIONS_C

#include "includes.h"
#define ACTION_DEBUG_

// Will I need this?
void A_DoNothing()
{
    // #ifdef ACTION_DEBUG_
    // printf("%s", __func__);
    // #endif
    return;
}

void A_ScopeDecrease()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.scope > 0) GVARS.scope -= 1;
}

void A_SelectChar()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    selectChar(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir());
}

void A_MoveCursor()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (CursorMoveDir(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir())) Deselect();
}

void A_MoveCursorByToken()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    assert(!"TODO: A_MoveCursor_Token");
}

void A_MoveCursorToStart()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, 0);
    Deselect();
}

void A_MoveCursorToEnd()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, strlen(gapStrToStr(sheet[GVARS.selectedCellIndex].gapStr, CELL_TEXT_LENGTH)));
    Deselect();
}

void A_LoadTimes()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (loadTimes()) {
        UpdateScores();
        printf("Loaded times from times/times.txt\n");
    }
}

void A_SaveTimes()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    saveTimes();
    printf("Saved times to times/times.txt\n");
}

void A_ExportTimes()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    ExportToBBCode();
    printf("Exported to BBCode\n");
}

void A_ClearTimes()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    ClearTimes();
    UpdateScores();
    printf("Sheet Cleared\n");
}

void A_Deselect_Or_Undo_Backout()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selection.exists) {
        Deselect();
    } else {
        // TODO: Undo action
        GVARS.scope = SCOPE_SHEET;
    }
}

void A_Overwrite_UpdateScore()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    CellOverwriteHandler();
    EnterNavigationHandler();
    UpdateScores();
    A_ScopeDecrease();
    if (GVARS.selectedCellIndex == 0) {
        GVARS.scope = SCOPE_OVERVIEW;
    } else GVARS.scope = SCOPE_SHEET;
}

void A_Copy()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
}

void A_Cut()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
}

void A_Paste()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
    Deselect();
}

void A_DeleteCellText()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
}

void A_DeleteSelection()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    printf("Not Implemented: %s", __func__);
//     MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, GVARS.selection.end);
//     while (sheet[GVARS.selectedCellIndex].gapStr.cStart != GVARS.selection.start) {
//         deleteCharAtCursor(&sheet[GVARS.selectedCellIndex].gapStr);
//     }
}

void A_DeleteChar()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selection.exists) A_DeleteSelection();
    deleteCharAtCursor(&sheet[GVARS.selectedCellIndex].gapStr);
}

void A_NavigateLeft()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selectedCellIndex % 3 == 2)
    GVARS.selectedCellIndex = GVARS.selectedCellIndex - 1;
}

void A_NavigateRight()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selectedCellIndex % 3 == 1) 
    GVARS.selectedCellIndex = GVARS.selectedCellIndex + 1;
}

void A_NavigateUp()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selectedCellIndex < 3) return;
    GVARS.selectedCellIndex = GVARS.selectedCellIndex - 3;
}

void A_NavigateDown()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selectedCellIndex > CELL_COUNT - 6) return;
    GVARS.selectedCellIndex = GVARS.selectedCellIndex + 3;
}

void A_Delete()
{
    if (GVARS.selection.exists) {
        A_DeleteSelection();
    } else A_DeleteChar();
}

// List of action functions
void (*Action[])() = {
    A_DoNothing,
    A_ScopeDecrease,
    A_SelectChar,
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
    A_Copy,
    A_Cut,
    A_Paste,
    A_Delete,
    A_DeleteCellText,
    A_DeleteSelection,
    A_DeleteChar,
    A_NavigateLeft,
    A_NavigateRight,
    A_NavigateUp,
    A_NavigateDown,
};

// List of Actions
typedef enum Actions {
    A_DONOTHING,
    A_SCOPEDECREASE,
    A_SELECTCHAR,
    A_MOVECURSOR,
    A_MOVECURSORBYTOKEN,
    A_MOVECURSORTOSTART,
    A_MOVECURSORTOEND,
    A_LOADTIMES,
    A_SAVETIMES,
    A_EXPORTTIMES,
    A_CLEARTIMES,
    A_DESELECT_OR_UNDO_BACKOUT,
    A_OVERWRITE_UPDATESCORE,
    A_COPY,
    A_CUT,
    A_PASTE,
    A_DELETE,
    A_DELETECELLTEXT,
    A_DELETESELECTION,
    A_DELETECHAR,
    A_NAVIGATELEFT,
    A_NAVIGATERIGHT,
    A_NAVIGATEUP,
    A_NAVIGATEDOWN,

    ACTION_COUNT
} Actions;

const char* actionnames[] = {
    "A_DONOTHING",
    "A_SCOPEDECREASE",
    "A_SELECTCHAR",
    "A_MOVECURSOR",
    "A_MOVECURSORBYTOKEN",
    "A_MOVECURSORTOSTART",
    "A_MOVECURSORTOEND",
    "A_LOADTIMES",
    "A_SAVETIMES",
    "A_EXPORTTIMES",
    "A_CLEARTIMES",
    "A_DESELECT_OR_UNDO_BACKOUT",
    "A_OVERWRITE_UPDATESCORE",
    "A_COPY",
    "A_CUT",
    "A_PASTE",
    "A_DELETE",
    "A_DELETECELLTEXT",
    "A_DELETESELECTION",
    "A_DELETECHAR",
    "A_NAVIGATELEFT",
    "A_NAVIGATERIGHT",
    "A_NAVIGATEUP",
    "A_NAVIGATEDOWN",
};

const char* actionnames_humanreadable[] = {
    "N/A",
    "Scope decrease",
    "Select char",
    "Move cursor",
    "Move cursor by token",
    "Move cursor to start",
    "Move cursor to end",
    "Load times",
    "Save times",
    "Export times",
    "Clear times",
    "Deselect or undo and backout",
    "Overwrite and update score",
    "Copy",
    "Cut",
    "Paste",
    "Delete",
    "Delete cell text",
    "Delete selection",
    "Delete char",
    "Navigate left",
    "Navigate right",
    "Navigate up",
    "Navigate down",
};

typedef enum Keys {
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_ENTER,
    K_ESCAPE,
    K_DELETE,
    K_BACKSPACE,
    K_TAB,
    K_HOME,
    K_END,
    K_C,
    K_X,
    K_V,
    K_L,
    K_S,
    K_B,

    KEY_COUNT
} Keys;

const int R_Keys[KEY_COUNT] = {
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_DELETE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_HOME,
    KEY_END,
    KEY_C,
    KEY_X,
    KEY_V,
    KEY_L,
    KEY_S,
    KEY_B,
};

const char* keynames[] = {
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN",
    "ENTER",
    "ESCAPE",
    "DELETE",
    "BACKSPACE",
    "TAB",
    "HOME",
    "END",
    "C",
    "X",
    "V",
    "L",
    "S",
    "B",
};

Actions ActionTable[3][KEY_COUNT * 4];

#endif