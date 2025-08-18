#ifndef ACTIONS_H
#define ACTIONS_H

#define ACTION_LIST \
    ACTION(A_DONOTHING, A_DoNothing, "N/A") \
    ACTION(A_SCOPEDECREASE, A_ScopeDecrease, "Scope decrease") \
    ACTION(A_SELECTCHAR, A_SelectChar, "Select char") \
    ACTION(A_SELECTALL, A_SelectAll, "Select all cell text") \
    ACTION(A_SELECTALLATCURSORTOWARDSDIR, A_SelectAllAtCursorTowardsDir, "Select from cursor by dir") \
    ACTION(A_MOVECURSOR, A_MoveCursor, "Move cursor") \
    ACTION(A_MOVECURSORBYTOKEN, A_MoveCursorByToken, "Move cursor by token") \
    ACTION(A_MOVECURSORTOSTART, A_MoveCursorToStart, "Move cursor to start") \
    ACTION(A_MOVECURSORTOEND, A_MoveCursorToEnd, "Move cursor to end") \
    ACTION(A_LOADTIMES, A_LoadTimes, "Load times") \
    ACTION(A_SAVETIMES, A_SaveTimes, "Save times") \
    ACTION(A_EXPORTTIMES, A_ExportTimes, "Export to BBCode") \
    ACTION(A_CLEARTIMES, A_ClearTimes, "Clear times") \
    ACTION(A_DESELECT_OR_UNDO_BACKOUT, A_Deselect_Or_Undo_Backout, "Deselect or undo and backout") \
    ACTION(A_OVERWRITE_UPDATESCORE, A_Overwrite_UpdateScore, "Overwrite and update score") \
    ACTION(A_NAVIGATETONEXTCELL, A_NavigateToNextCell, "Navigate to next cell") \
    ACTION(A_COPY, A_Copy, "Copy") \
    ACTION(A_COPY_ALL, A_Copy_All, "Copy all cell text") \
    ACTION(A_CUT, A_Cut, "Cut") \
    ACTION(A_CUT_ALL, A_Cut_All, "Cut all cell text") \
    ACTION(A_PASTE, A_Paste, "Paste") \
    ACTION(A_PASTE_INTO_CELL, A_PasteIntoCell, "Overwrite cell with paste") \
    ACTION(A_DELETE, A_Delete, "Delete") \
    ACTION(A_DELETECELLTEXT, A_DeleteCellText, "Delete cell text") \
    ACTION(A_DELETECELLTEXTANDENTERINTO, A_DeleteCellTextAndEnterInto, "Delete cell text; enter cell") \
    ACTION(A_DELETESELECTION, A_DeleteSelection, "Delete selection") \
    ACTION(A_DELETECHAR, A_DeleteChar, "Delete char") \
    ACTION(A_NAVIGATELEFT, A_NavigateLeft, "Navigate left") \
    ACTION(A_NAVIGATERIGHT, A_NavigateRight, "Navigate right") \
    ACTION(A_NAVIGATEUP, A_NavigateUp, "Navigate up") \
    ACTION(A_NAVIGATEDOWN, A_NavigateDown, "Navigate down") \
    ACTION(A_SWAPVETOCOLOR, A_SwapVetoColor, "Swap veto text color") \
    ACTION(A_RESET_TEXT_COLOR, A_ResetTextColor, "Reset cell text color") \

typedef enum Action {
    #define ACTION(a,b,c) a,
        ACTION_LIST
    #undef ACTION
        ACTION_COUNT
} Action;

extern Action ActionTable[3][KEY_COMBO_COUNT];
extern void (*Action_Function[ACTION_COUNT])();

#endif