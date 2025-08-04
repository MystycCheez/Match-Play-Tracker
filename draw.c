#ifndef DRAW_C
#define DRAW_C

#include "includes.h"

void DrawCellBorders(size_t cellIndex)
{
    if (cellIndex == 0) return;
    Vector2 cellOrigin = indexToXY(cellIndex);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y, GVARS.cellWidth, GVARS.cellHeight}, 3.0, RAYWHITE);
}

void DrawCursor(Cell *sheet, size_t cellIndex, Font font)
{
    if (cellIndex == 0) return;
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    float span = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH), GVARS.fontSize, 1).x;
    float offset = MeasureTextEx(font, sheet[cellIndex].gapStr.str, GVARS.fontSize, 1).x;
    pos.x += (GVARS.cellWidth / 2) - (span / 2) + offset + 1;
    pos.y += 2;
    DrawLineEx(pos, (Vector2){pos.x, pos.y + GVARS.cellHeight - 6}, 1.0, LIGHTGRAY);
}

void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + (GVARS.cellWidth / 2) - (size.x / 2);
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + fontSize / 2;
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell, size_t i)
{
    switch (cell.alignment) {
    case ALIGN_LEFT:
        DrawTextLeftAligned(font, CalcTextPos(pos, i), fontSize, spacing, cell);
        break;
    case ALIGN_CENTER:
        DrawTextCentered(font, CalcTextPos(pos, i), fontSize, spacing, cell);
        break;
    case ALIGN_RIGHT:
        assert(!"TODO: ALIGN_RIGHT");
        break;
    }
}

void DrawTextHighlight(Cell *sheet, size_t cellIndex, Font font)
{
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    size_t selectionLen = GVARS.selection.end - GVARS.selection.start;
    char *selectedText = gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH) + GVARS.selection.start;
    selectedText[selectionLen] = 0;
    float selectionSpan = MeasureTextEx(font, selectedText, GVARS.fontSize, 1).x;

    float cellTextSpan = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH), GVARS.fontSize, 1).x;
    float offset = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, GVARS.selection.start), GVARS.fontSize, 1).x;

    pos.x += (GVARS.cellWidth / 2) - (cellTextSpan / 2) + offset;
    DrawRectangleRec((Rectangle){pos.x, pos.y, selectionSpan + 2, GVARS.cellHeight}, COLOR_HIGHLIGHT);
}

#endif