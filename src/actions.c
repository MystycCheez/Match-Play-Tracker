#ifndef ACTIONS_C
#define ACTIONS_C

#define ACTION_DEBUG_

#include "headers.h"

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

void A_SelectAll()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    A_MoveCursorToStart();
    while (selectChar(&sheet[GVARS.selectedCellIndex].gapStr, DIR_RIGHT)) {
        A_DoNothing();
    }
}

void A_SelectAllAtCursorTowardsDir()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    bool dir = getMoveDir();

    if (GVARS.selection.exists) {
        if (dir == DIR_RIGHT) {
            if (GVARS.selection.end < gapStrLen(sheet[GVARS.selectedCellIndex].gapStr)) {
                goto select;
            } else {
                MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, GVARS.selection.end);
                Deselect();
            }
        } else if (dir == DIR_LEFT) {
            if (GVARS.selection.end > sheet[GVARS.selectedCellIndex].gapStr.cStart) {
                goto select;
            } else {
                MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, GVARS.selection.start);
                Deselect();
            }
        }
    } else {
        select: 
        while (selectChar(&sheet[GVARS.selectedCellIndex].gapStr, dir)) {
            A_DoNothing();
        }
    }
}

void A_MoveCursor()
{
    #ifdef ACTION_DEBUG_
    printf("%s\n", __func__);
    #endif
    CursorMoveDir(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir());
    Deselect();
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
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
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
    Deselect();
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

void (*Action_Function[ACTION_COUNT])() = {
    #define ACTION(a,b,c) b,
        ACTION_LIST
    #undef ACTION
};

Action ActionTable[3][KEY_COMBO_COUNT];

#endif