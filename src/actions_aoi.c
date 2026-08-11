#ifndef ACTIONS_AOI_C
#define ACTIONS_AOI_C

#include <stdio.h>

#include <SDL3/SDL_clipboard.h>
#include <aoi/aoi.h>

#include "structs.h"

#include "text.h"
#include "get.h"
#include "io.h"
#include "functions.h"
#include "handler.h"
#include "init.h"

#include "actions_aoi.h"

void A_DoNothing(aoiData* Data);
void A_ScopeDecrease(aoiData* Data);
void A_MoveCursor(aoiData* Data);
void A_MoveCursorToStart(aoiData* Data);
void A_MoveCursorToEnd(aoiData* Data);
void A_LoadTimes(aoiData* Data);
void A_SaveTimes(aoiData* Data);
void A_ExportTimes(aoiData* Data);
void A_ClearTimes(aoiData* Data);
void A_Deselect(aoiData* Data);
void A_Overwrite_UpdateScore(aoiData* Data);
void A_SelectChar(aoiData* Data);
void A_SelectAll(aoiData* Data);
void A_SelectAllAtCursorTowardsDir(aoiData* Data);
void A_MoveCursorByToken(aoiData* Data);
void A_NavigateToNextCell(aoiData* Data);
void A_Copy(aoiData* Data);
void A_Copy_All(aoiData* Data);
void A_Cut(aoiData* Data);
void A_Cut_All(aoiData* Data);
void A_Paste(aoiData* Data);
void A_PasteIntoCell(aoiData* Data);
void A_Delete(aoiData* Data);
void A_DeleteCellText(aoiData* Data);
void A_DeleteCellTextAndEnterInto(aoiData* Data);
void A_DeleteSelection(aoiData* Data);
void A_DeleteChar(aoiData* Data);
void A_NavigateLeft(aoiData* Data);
void A_NavigateRight(aoiData* Data);
void A_NavigateUp(aoiData* Data);
void A_NavigateDown(aoiData* Data);
void A_SwapVetoColor(aoiData* Data);
void A_ResetTextColor(aoiData* Data);
void A_SwapGameText(aoiData* Data);
void A_ToggleExpansion(aoiData* Data);
void A_ReloadGameText(aoiData* Data);

// NOTE: I am fairly sure this is un-needed, but keeping it for compatibility purposes for now
void A_DoNothing(aoiData* Data)
{
    (void)Data;
    return;
}

void A_ScopeDecrease(aoiData* Data)
{
    uint16_t scope = GetBindingEntry(Data, "Scope")->patternElement;
    if (scope > 0) SetBindings(Data, {"Scope", scope -= 1});
}

// void A_SelectChar(aoiData* Data);
// void A_SelectAll();
// void A_SelectAllAtCursorTowardsDir();

void A_MoveCursor(aoiData* Data)
{
    TextStruct* Text = GetUserData(Data, "Text");
    int* dir = GetUserData(Data, "MoveDir");
    Text->cursor += *dir;
}

// void A_MoveCursorByToken();

void A_MoveCursorToStart(aoiData* Data)
{
    TextStruct* Text = GetUserData(Data, "Text");
    Text->cursor = 0;
}

void A_MoveCursorToEnd(aoiData* Data)
{
    TextStruct* Text = GetUserData(Data, "Text");
    Text->cursor = Text->len - 1;
}

void A_LoadTimes(aoiData* Data)
{
    (void)Data;
    if (loadTimes()) {
        UpdateScores();
        printf("Loaded times from times/times.txt\n");
    }
}

void A_SaveTimes(aoiData* Data)
{
    (void)Data;
    saveTimes();
    printf("Saved times to times/times.txt\n");
}

void A_ExportTimes(aoiData* Data)
{
    (void)Data;
    ExportToBBCode();
    printf("Exported to BBCode\n");
    printf("Copied to clipboard\n");
}

void A_ClearTimes(aoiData* Data)
{
    (void)Data;
    ClearTimes();
    UpdateScores();
    printf("Sheet Cleared\n");
}

void A_Deselect(aoiData* Data)
{
    TextStruct* Text = GetUserData(Data, "Text");
    Text->anchor = -1;
}

void A_Overwrite_UpdateScore(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");
    if (sheet->activeCell % 3 == 0) {

    } else {
        CellOverwriteHandler();
        UpdateScores();
    }
    A_Deselect(Data);
    A_NavigateToNextCell(Data);
    A_ScopeDecrease(Data);
    if (sheet->activeCell == 0) {
        SetBindings(Data, {"Scope", SCOPE_OVERVIEW});
    } else SetBindings(Data, {"Scope", SCOPE_SHEET});
}

void A_NavigateToNextCell(aoiData* Data)
{
    (void)Data;
    EnterNavigationHandler();
}

// void A_Copy(aoiData* Data);

void A_Copy_All(aoiData* Data)
{
    Cell* cell = GetUserData(Data, "ActiveCell");
    if (!cell) return;
    SDL_SetClipboardText(cell->Text.str);
}

// void A_Cut(aoiData* Data);
// void A_Cut_All(aoiData* Data);
// void A_Paste(aoiData* Data);

void A_PasteIntoCell(aoiData* Data)
{
    TextStruct* Text = GetUserData(Data, "Text");
    SetText(Text, SDL_GetClipboardText());

    SetBindings(Data, {"Scope", SCOPE_CELL});
}

// void A_DeleteCellText(aoiData* Data);
// void A_DeleteCellTextAndEnterInto();
// void A_DeleteSelection();
// void A_DeleteChar();

void A_NavigateLeft(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");

    bool conditions = 
    (sheet->cells[sheet->activeCell - 1].selectable == true) && 
    (sheet->activeCell % 3 >= 1);

    if (conditions) updateSheetIndex(sheet->activeCell - 1);
}

void A_NavigateRight(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");

    bool conditions = 
    (sheet->cells[sheet->activeCell + 1].selectable == true) && 
    (sheet->activeCell % 3 <= 1);

    if (conditions) updateSheetIndex(sheet->activeCell + 1);
}

void A_NavigateUp(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");

    bool conditions = 
    (sheet->cells[sheet->activeCell - 3].selectable == true) && 
    (sheet->activeCell % 3 >= 1);

    if (conditions) updateSheetIndex(sheet->activeCell - 3);
}

void A_NavigateDown(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");

    bool conditions = 
    (sheet->cells[sheet->activeCell + 3].selectable == true) && 
    (sheet->activeCell % 3 <= CELL_COUNT - 6);

    if (conditions) updateSheetIndex(sheet->activeCell + 3);
}

void A_SwapVetoColor(aoiData* Data);

void A_ResetTextColor(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");

    if (sheet->activeCell % 3 == 0) {
        sheet->cells[sheet->activeCell].color = COLOR_LEVEL;
    } else {
        sheet->cells[sheet->activeCell].color = WHITE;
    }
}

void A_SwapGameText(aoiData* Data)
{
    bool* game = GetUserData(Data, "Game");

    *game = !*game;
    setGameText();
}

void A_ToggleExpansion(aoiData* Data)
{
    Window_Elements* Window = GetUserData(Data, "Window");

    Window->expand = !Window->expand;
    if (Window->expand) {
        Window->Height += BASE_CELL_HEIGHT * 2 * Window->scaleDPI.y;
    } else {
        Window->Height -= BASE_CELL_HEIGHT * 2 * Window->scaleDPI.y;
    }
    SetWindowSize(Window->Width, Window->Height);
}

void A_ReloadGameText(aoiData* Data)
{
    (void)Data;
    setGameText();
    unselectCells();
}

#endif