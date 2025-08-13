#ifndef ACTIONS_C
#define ACTIONS_C

#include "includes.h"

// Will I need this?
void A_DoNothing()
{
    return;
}

void A_ScopeDecrease()
{
    GVARS.scope = max(GVARS.scope - 1, 0);
}

void A_SelectChar()
{
    selectChar(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir());
}

void A_MoveCursor()
{
    if (CursorMoveDir(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir())) Deselect();
}

void A_MoveCursorByToken()
{
    assert(!"TODO: A_MoveCursor_Token");
}

void A_MoveCursorToStart()
{
    MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, 0);
    Deselect();
}

void A_MoveCursorToEnd()
{
    MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, strlen(gapStrToStr(sheet[GVARS.selectedCellIndex].gapStr, CELL_TEXT_LENGTH)));
    Deselect();
}

void A_LoadTimes()
{
    if (loadTimes()) {
        UpdateScores();
        printf("Loaded times from times/times.txt\n");
    }
}

void A_SaveTimes()
{
    saveTimes();
    printf("Saved times to times/times.txt\n");
}

void A_ExportTimes()
{
    ExportToBBCode();
    printf("Exported to BBCode\n");
}

void A_ClearTimes()
{
    ClearTimes();
    UpdateScores();
    printf("Sheet Cleared\n");
}

void A_Deselect_Or_UndoAndBackout()
{
    if (GVARS.selection.exists) {
        Deselect();
    } else {
        // TODO: Undo action
        GVARS.scope = SCOPE_SHEET;
    }
}

void A_OverwriteAndUpdate()
{
    CellOverwriteHandler();
    EnterNavigationHandler();
    UpdateScores();
    if (GVARS.selectedCellIndex == 0) A_ScopeDecrease();
}

void A_Copy()
{
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
}

void A_Cut()
{
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
}

void A_Paste()
{
    placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
}

void A_DeleteCellText()
{
    OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
}

void A_DeleteSelection()
{
    // TODO
}

void A_DeleteChar()
{
    if (GVARS.selection.exists) A_DeleteSelection();
    deleteCharAtCursor(&sheet[GVARS.selectedCellIndex].gapStr);
}


// List of actions
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
    A_Deselect_Or_UndoAndBackout,
    A_OverwriteAndUpdate,
    A_Copy,
    A_Cut,
    A_Paste,
    A_DeleteCellText,
    A_DeleteSelection,
    A_DeleteChar,
};

// Do I need this enumeration? Leaning likely yes
// List of Actions for ("dispatch table" ?)
//
// Do I need unique actions for functions with only slight differences?
// For example, functions with a direction (i.e. A_MOVE_CURSOR_L_TOKEN & A_MOVE_CURSOR_R_TOKEN)
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
    A_DESELECT_OR_UNDOANDBACKOUT,
    A_OVERWRITEANDUPDATE,
    A_COPY,
    A_CUT,
    A_PASTE,
    A_DELETECELLTEXT,
    A_DELETESELECTION,
    A_DELETECHAR,

    ACTION_COUNT // Do not use!
} Actions;

typedef enum Keys {
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_ENTER,
    K_ESCAPE,
    K_DELETE,
    K_BACKSPACE,
    K_C,
    K_X,
    K_V,
    K_L,
    K_S,
    K_B,

    KEY_COUNT
} Keys;

const int keys[KEY_COUNT] = {
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_DELETE,
    KEY_BACKSPACE,
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
    "C",
    "X",
    "V",
    "L",
    "S",
    "B",
};

Actions ActionTable[3][KEY_COUNT * 3];

#endif