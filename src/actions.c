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
    
    selectChar(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir());
}

void A_SelectAll()
{
    
    A_MoveCursorToStart();
    while (selectChar(&sheet[GVARS.selectedCellIndex].gapStr, DIR_RIGHT)) {
        A_DoNothing();
    }
}

void A_SelectAllAtCursorTowardsDir()
{
    
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
    
    CursorMoveDir(&sheet[GVARS.selectedCellIndex].gapStr, getMoveDir());
    Deselect();
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
    
    if (GVARS.selection.exists) {
        Deselect();
    } else {
        OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_SHEET;
    }
}

void A_Overwrite_UpdateScore()
{
    
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
    
    EnterNavigationHandler();
}

void A_Copy()
{
    
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
}

void A_Copy_All()
{
    
    A_SelectAll();
    A_Copy();
    Deselect();
}

void A_Cut()
{
    
    CopyText(sheet[GVARS.selectedCellIndex].gapStr);
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
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
    
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
    placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
    Deselect();
}

void A_PasteIntoCell()
{
        
    A_DeleteCellText();
    placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
    GVARS.scope = SCOPE_CELL;
}

void A_DeleteCellText()
{
    
    OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
}

void A_DeleteCellTextAndEnterInto()
{
    
    OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    CellOverwriteHandler();
    UpdateScores();
    GVARS.scope = SCOPE_CELL;
}

void A_DeleteSelection()
{
    
    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
    Deselect();
}

void A_DeleteChar()
{
    
    if (GVARS.selection.exists) A_DeleteSelection();
    deleteCharAtCursor(&sheet[GVARS.selectedCellIndex].gapStr);
}

void A_NavigateLeft()
{
    
    if (GVARS.selectedCellIndex % 3 == 2)
    GVARS.selectedCellIndex = GVARS.selectedCellIndex - 1;
}

void A_NavigateRight()
{
    
    if (GVARS.selectedCellIndex % 3 == 1) 
    GVARS.selectedCellIndex = GVARS.selectedCellIndex + 1;
}

void A_NavigateUp()
{
    
    if (GVARS.selectedCellIndex < 3) return;
    GVARS.selectedCellIndex = GVARS.selectedCellIndex - 3;
}

void A_NavigateDown()
{
    
    if (GVARS.selectedCellIndex > CELL_COUNT - 6) return;
    GVARS.selectedCellIndex = GVARS.selectedCellIndex + 3;
}

void A_Delete()
{
    
    if (GVARS.selection.exists) {
        A_DeleteSelection();
    } else A_DeleteChar();
}

void A_SwapVetoColor()
{
    
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

void A_ResetTextColor()
{
    
    sheet[GVARS.selectedCellIndex].color = WHITE;
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
        Window.Height += BASE_CELL_HEIGHT * 2 * GVARS.scaleDPI.y;
    } else {
        Window.Height -= BASE_CELL_HEIGHT * 2 * GVARS.scaleDPI.y;
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