#ifndef ACTIONS_C
#define ACTIONS_C

#include "includes.h"
#define ACTION_DEBUG_

void A_DoNothing()
{
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
        OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_SHEET;
    }
}

void A_Overwrite_UpdateScore()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    CellOverwriteHandler();
    A_NavigateToNextCell();
    UpdateScores();
    A_ScopeDecrease();
    if (GVARS.selectedCellIndex == 0) {
        GVARS.scope = SCOPE_OVERVIEW;
    } else GVARS.scope = SCOPE_SHEET;
}

void A_NavigateToNextCell()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    EnterNavigationHandler();
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

void A_DeleteCellTextAndEnterInto()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
    GVARS.scope = SCOPE_CELL;
}

void A_DeleteSelection()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
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
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    if (GVARS.selection.exists) {
        A_DeleteSelection();
    } else A_DeleteChar();
}

void A_SwapVetoColor()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    GVARS.vetoFlag = !GVARS.vetoFlag;
    for (size_t i = 4; i < LEVEL_COUNT * 2; i++) {
        if (!(i % 3 == 0)) {
            for (size_t n = 0; n < 3; n++) {
                if (CompareSpecialText(gapStrToStr(sheet[i].gapStr, CELL_TEXT_LENGTH)) == TEXT_VETO) {
                    sheet[i].color = GVARS.vetoFlag ? COLOR_LEVEL : WHITE;  
                }
            }
        }
    }
    
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
    A_NAVIGATETONEXTCELL,
    A_COPY,
    A_CUT,
    A_PASTE,
    A_DELETE,
    A_DELETECELLTEXT,
    A_DELETECELLTEXTANDENTERINTO,
    A_DELETESELECTION,
    A_DELETECHAR,
    A_NAVIGATELEFT,
    A_NAVIGATERIGHT,
    A_NAVIGATEUP,
    A_NAVIGATEDOWN,
    A_SWAPVETOCOLOR,

    ACTION_COUNT
} Actions;

Actions ActionTable[3][KEY_COMBO_COUNT];

#endif