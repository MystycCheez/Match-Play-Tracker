#ifndef STRUCTS_H
#define STRUCTS_H

#include <aoi/aoi.h>

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

#endif