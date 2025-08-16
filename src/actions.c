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
    while (selectChar(&sheet[GVARS.selectedCellIndex].gapStr, dir)) {
        A_DoNothing();
    }
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

const char* GetActionText(int index)
{
    switch (index) {
        case A_DONOTHING:                   return "A_DONOTHING";
        case A_SCOPEDECREASE:               return "A_SCOPEDECREASE";
        case A_SELECTCHAR:                  return "A_SELECTCHAR";
        case A_SELECTALL:                   return "A_SELECTALL";
        case A_SELECTALLATCURSORTOWARDSDIR: return "A_SELECTALLATCURSORTOWARDSDIR";
        case A_MOVECURSOR:                  return "A_MOVECURSOR";
        case A_MOVECURSORBYTOKEN:           return "A_MOVECURSORBYTOKEN";
        case A_MOVECURSORTOSTART:           return "A_MOVECURSORTOSTART";
        case A_MOVECURSORTOEND:             return "A_MOVECURSORTOEND";
        case A_LOADTIMES:                   return "A_LOADTIMES";
        case A_SAVETIMES:                   return "A_SAVETIMES";
        case A_EXPORTTIMES:                 return "A_EXPORTTIMES";
        case A_CLEARTIMES:                  return "A_CLEARTIMES";
        case A_DESELECT_OR_UNDO_BACKOUT:    return "A_DESELECT_OR_UNDO_BACKOUT";
        case A_OVERWRITE_UPDATESCORE:       return "A_OVERWRITE_UPDATESCORE";
        case A_NAVIGATETONEXTCELL:          return "A_NAVIGATETONEXTCELL";
        case A_COPY:                        return "A_COPY";
        case A_CUT:                         return "A_CUT";
        case A_PASTE:                       return "A_PASTE";
        case A_DELETE:                      return "A_DELETE";
        case A_DELETECELLTEXT:              return "A_DELETECELLTEXT";
        case A_DELETECELLTEXTANDENTERINTO:  return "A_DELETECELLTEXTANDENTERINTO";
        case A_DELETESELECTION:             return "A_DELETESELECTION";
        case A_DELETECHAR:                  return "A_DELETECHAR";
        case A_NAVIGATELEFT:                return "A_NAVIGATELEFT";
        case A_NAVIGATERIGHT:               return "A_NAVIGATERIGHT";
        case A_NAVIGATEUP:                  return "A_NAVIGATEUP";
        case A_NAVIGATEDOWN:                return "A_NAVIGATEDOWN";
        case A_SWAPVETOCOLOR:               return "A_SWAPVETOCOLOR";
        default: assert(!"Not a valid action!\n");
    }
}

const char* GetHumanReadableActionText(int index)
{
    switch (index) {
        case A_DONOTHING:                   return "N/A";
        case A_SCOPEDECREASE:               return "Scope decrease";
        case A_SELECTCHAR:                  return "Select char";
        case A_SELECTALL:                   return "Select all cell text";
        case A_SELECTALLATCURSORTOWARDSDIR: return "Select from cursor by dir";
        case A_MOVECURSOR:                  return "Move cursor";
        case A_MOVECURSORBYTOKEN:           return "Move cursor by token";
        case A_MOVECURSORTOSTART:           return "Move cursor to start";
        case A_MOVECURSORTOEND:             return "Move cursor to end";
        case A_LOADTIMES:                   return "Load times";
        case A_SAVETIMES:                   return "Save times";
        case A_EXPORTTIMES:                 return "Export times";
        case A_CLEARTIMES:                  return "Clear times";
        case A_DESELECT_OR_UNDO_BACKOUT:    return "Deselect or undo and backout";
        case A_OVERWRITE_UPDATESCORE:       return "Overwrite and update score";
        case A_NAVIGATETONEXTCELL:          return "Navigate to next cell";
        case A_COPY:                        return "Copy";
        case A_CUT:                         return "Cut";
        case A_PASTE:                       return "Paste";
        case A_DELETE:                      return "Delete";
        case A_DELETECELLTEXT:              return "Delete cell text";
        case A_DELETECELLTEXTANDENTERINTO:  return "Delete cell text; enter cell";
        case A_DELETESELECTION:             return "Delete selection";
        case A_DELETECHAR:                  return "Delete char";
        case A_NAVIGATELEFT:                return "Navigate left";
        case A_NAVIGATERIGHT:               return "Navigate right";
        case A_NAVIGATEUP:                  return "Navigate up";
        case A_NAVIGATEDOWN:                return "Navigate down";
        case A_SWAPVETOCOLOR:               return "Swap veto text color";
        default: assert(!"Not a valid action!\n");
    }
}

Actions ActionTable[3][KEY_COMBO_COUNT];

#endif