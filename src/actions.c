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
    if (GVARS.scope > 0) GVARS.scope -= 1;
}

void A_SelectChar()
{
    selectChar(&Sheet.cell->gapStr, getMoveDir());
}

void A_SelectAll()
{
    A_MoveCursorToStart();
    while (selectChar(&Sheet.cell->gapStr, DIR_RIGHT)) {
        A_DoNothing();
    }
}

void A_SelectAllAtCursorTowardsDir()
{
    bool dir = getMoveDir();

    if (Sheet.selection.exists) {
        if (dir == DIR_RIGHT) {
            if (Sheet.selection.end < gapStrLen(Sheet.cell->gapStr)) {
                goto select;
            } else {
                MoveCursorToIndex(&Sheet.cell->gapStr, Sheet.selection.end);
                Deselect();
            }
        } else if (dir == DIR_LEFT) {
            if (Sheet.selection.end > Sheet.cell->gapStr.cStart) {
                goto select;
            } else {
                MoveCursorToIndex(&Sheet.cell->gapStr, Sheet.selection.start);
                Deselect();
            }
        }
    } else {
        select: 
        while (selectChar(&Sheet.cell->gapStr, dir)) {
            A_DoNothing();
        }
    }
}

void A_MoveCursor()
{
    CursorMoveDir(&Sheet.cell->gapStr, getMoveDir());
    Deselect();
}

void A_MoveCursorByToken()
{
    assert(!"TODO: A_MoveCursor_Token");
}

void A_MoveCursorToStart()
{
    MoveCursorToIndex(&Sheet.cell->gapStr, 0);
    Deselect();
}

void A_MoveCursorToEnd()
{
    MoveCursorToIndex(&Sheet.cell->gapStr, strlen(gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH)));
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
    printf("Copied to clipboard\n");
}

void A_ClearTimes()
{
    ClearTimes();
    UpdateScores();
    printf("Sheet Cleared\n");
}

void A_Deselect_Or_Undo_Backout()
{
    if (Sheet.selection.exists) {
        Deselect();
    } else {
        OverwriteStr(&Sheet.cell->gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_SHEET;
    }
}

void A_Overwrite_UpdateScore()
{
    CellOverwriteHandler();
    A_NavigateToNextCell();
    UpdateScores();
    A_ScopeDecrease();
    if (Sheet.index == 0) {
        GVARS.scope = SCOPE_OVERVIEW;
    } else GVARS.scope = SCOPE_SHEET;
}

void A_NavigateToNextCell()
{
    EnterNavigationHandler();
}

void A_Copy()
{
    CopyText(Sheet.cell->gapStr);
}

void A_Copy_All()
{
    A_SelectAll();
    A_Copy();
    Deselect();
}

void A_Cut()
{
    CopyText(Sheet.cell->gapStr);
    DeleteSelection(&Sheet.cell->gapStr);
}

void A_Cut_All()
{
    A_SelectAll();
    A_Copy();
    Deselect();
    A_DeleteCellText();
}

void A_Paste()
{
    DeleteSelection(&Sheet.cell->gapStr);
    placeString(&Sheet.cell->gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
    Deselect();
}

void A_PasteIntoCell()
{
    A_DeleteCellText();
    placeString(&Sheet.cell->gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
    GVARS.scope = SCOPE_CELL;
}

void A_DeleteCellText()
{
    OverwriteStr(&Sheet.cell->gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
}

void A_DeleteCellTextAndEnterInto()
{
    OverwriteStr(&Sheet.cell->gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
    GVARS.scope = SCOPE_CELL;
}

void A_DeleteSelection()
{
    DeleteSelection(&Sheet.cell->gapStr);
    Deselect();
}

void A_DeleteChar()
{
    if (Sheet.selection.exists) A_DeleteSelection();
    deleteCharAtCursor(&Sheet.cell->gapStr);
}

void A_NavigateLeft()
{
    if (
        (Sheet.cellList[Sheet.index - 1].selectable == true) &&
        (Sheet.index % 3 >= 1)
    ) updateSheetIndex(Sheet.index - 1);
}

void A_NavigateRight()
{
    if (
        (Sheet.cellList[Sheet.index + 1].selectable == true) &&
        (Sheet.index % 3 <= 1) 
    ) updateSheetIndex(Sheet.index + 1);
}

void A_NavigateUp()
{
    if (
        (Sheet.cellList[Sheet.index - 3].selectable == true) &&
        (Sheet.index >= 3)
    ) updateSheetIndex(Sheet.index - 3);
}

void A_NavigateDown()
{
    if (
        (Sheet.cellList[Sheet.index + 3].selectable == true) &&
        (Sheet.index <= CELL_COUNT - 6)
    ) updateSheetIndex(Sheet.index + 3);
}

void A_Delete()
{
    if (Sheet.selection.exists) {
        A_DeleteSelection();
    } else A_DeleteChar();
}

void A_SwapVetoColor()
{
    GVARS.vetoFlag = !GVARS.vetoFlag;
    for (size_t i = 4; i < LEVEL_COUNT * 2; i++) {
        if (!(i % 3 == 0)) {
            for (size_t n = 0; n < 3; n++) {
                if (CompareSpecialText(gapStrToStr(Sheet.cellList[i].gapStr, CELL_TEXT_LENGTH)) == TEXT_VETO) {
                    Sheet.cellList[i].color = GVARS.vetoFlag ? COLOR_LEVEL : WHITE;  
                }
            }
        }
    }   
}

void A_ResetTextColor()
{
    Sheet.cell->color = WHITE;
}

void A_SwapGameText()
{
    GVARS.game = !GVARS.game;
    setGameText();
}

void A_ToggleExpansion()
{
    Window.expand = !Window.expand;
    if (Window.expand) {
        Window.Height += BASE_CELL_HEIGHT * 2 * Window.scaleDPI.y;
    } else {
        Window.Height -= BASE_CELL_HEIGHT * 2 * Window.scaleDPI.y;
    }
    SetWindowSize(Window.Width, Window.Height);
}

// Action function pointer list
void (*Action_Function[ACTION_COUNT])() = {
    #define ACTION(a,b,c) b,
        ACTION_LIST
    #undef ACTION
};

Action ActionTable[3][KEY_COMBO_COUNT];

#endif