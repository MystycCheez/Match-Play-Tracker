#ifndef DRAW_C
#define DRAW_C

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <aoi/aoi.h>

#include "raylib.h"

#include "globals.h"
#include "structs.h"
#include "functions.h"


void DrawSelectionBorders(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");
    UI_Elements* UI = GetUserData(Data, "UI");

    if (sheet->activeCellIndex == 0) return;
    Vector2 cellOrigin = indexToXY(sheet->activeCellIndex);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y + 1, UI->cellWidth - 1, UI->cellHeight - 1}, 2.0, RAYWHITE);
}

void DrawCursor(aoiData* Data)
{
    Sheet* sheet = GetUserData(Data, "Sheet");
    UI_Elements* UI = GetUserData(Data, "UI");

    if (sheet->activeCellIndex == 0) return;
    Vector2 pos = {0};
    pos = GetCellPos(sheet->activeCellIndex);
    float span = MeasureTextEx(UI->font, sheet->activeCell->Text.str, UI->fontSize, 1).x;
    float offset = MeasureTextEx(UI->font, sheet->activeCell->Text.str, UI->fontSize, 1).x;
    pos.x += (UI->cellWidth / 2) - (span / 2) + offset + 1;
    pos.y += 3;
    DrawLineEx(pos, (Vector2){pos.x, pos.y + UI->cellHeight - 5}, 1.0, LIGHTGRAY);
}

void DrawTextCentered(aoiData* Data, Vector2 pos, Cell cell)
{
    UI_Elements* UI = GetUserData(Data, "UI");

    Vector2 size = MeasureTextEx(UI->font, cell.Text.str, UI->fontSize, 1);

    pos.x = round(pos.x + (UI->cellWidth / 2) - (size.x / 2));
    pos.y = round(pos.y + (UI->cellHeight / 2) - (size.y / 2));

    DrawTextEx(UI->font, cell.Text.str, pos, UI->fontSize, 1, cell.color);
}

void DrawTextLeftAligned(aoiData* Data, Vector2 pos, Cell cell)
{
    UI_Elements* UI = GetUserData(Data, "UI");

    Vector2 size = MeasureTextEx(UI->font, cell.Text.str, UI->fontSize, 1);

    pos.x = round(pos.x + UI->fontSize / 2);
    pos.y = round(pos.y + (UI->cellHeight / 2) - (size.y / 2));

    DrawTextEx(UI->font, cell.Text.str, pos, UI->fontSize, 1, cell.color);
}

void DrawTextAligned(aoiData* Data, Cell cell, size_t cellIndex)
{
    switch (cell.alignment) {
    case ALIGN_LEFT:
        DrawTextLeftAligned(Data, GetCellPos(cellIndex), cell);
        break;
    case ALIGN_CENTER:
        DrawTextCentered(Data, GetCellPos(cellIndex), cell);
        break;
    case ALIGN_RIGHT:
        assert(!"TODO: ALIGN_RIGHT");
        break;
    default:
        assert(!"Value of cell.alignment is not valid!\n");
        exit(-1);
    }
}

// TODO: Figure out Selection
// void DrawTextHighlight()
// {
//     Vector2 pos = {0};
//     pos = GetCellPos(Sheet.index);

//     char* selectedText = strCreate(Sheet.selection.len + 1);
//     snprintf(selectedText, Sheet.selection.len + 1, "%s", Sheet.cell->Text.str + Sheet.selection.start);
//     snprintf(selectedText + Sheet.selection.len, 1, "%s", "\0");

//     float selectionSpan = MeasureTextEx(UI.font, selectedText, UI.fontSize, 1).x;
//     float cellTextSpan = MeasureTextEx(UI.font, Sheet.cell->Text.str, UI.fontSize, 1).x;

//     char* prefix = strCreate(Sheet.selection.start + 1);
//     memset(prefix, 0, Sheet.selection.start + 1);
//     snprintf(prefix, Sheet.selection.start + 1, "%s", Sheet.cell->Text.str);
//     float offset = MeasureTextEx(UI.font, prefix, UI.fontSize, 1).x;

//     pos.x += (UI.cellWidth / 2) - (cellTextSpan / 2) + offset;
//     pos.y += 1;
//     DrawRectangleRec((Rectangle){pos.x, pos.y, selectionSpan + 2, UI.cellHeight - 2}, COLOR_HIGHLIGHT);

//     free(selectedText);
//     free(prefix);
// }

void DrawTextHighlight()
{

}

#endif