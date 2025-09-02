#ifndef DRAW_C
#define DRAW_C

#include "headers.h"

void DrawSelectionBorders()
{
    if (Sheet.index == 0) return;
    Vector2 cellOrigin = indexToXY(Sheet.index);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y + 1, UI.cellWidth - 1, UI.cellHeight - 1}, 2.0, RAYWHITE);
}

void DrawCursor()
{
    if (Sheet.index == 0) return;
    Vector2 pos = {0};
    pos = CalcTextPos(pos, Sheet.index);
    char* text = gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH);
    float span = MeasureTextEx(UI.font, text, UI.fontSize, 1).x;
    float offset = MeasureTextEx(UI.font, Sheet.cell->gapStr.str, UI.fontSize, 1).x;
    pos.x += (UI.cellWidth / 2) - (span / 2) + offset + 1;
    pos.y += 2;
    DrawLineEx(pos, (Vector2){pos.x, pos.y + UI.cellHeight - 6}, 1.0, LIGHTGRAY);

    free(text);
}

void DrawTextCentered(Vector2 pos, Cell cell)
{
    char* text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(UI.font, text, UI.fontSize, 1);

    pos.x = pos.x + (UI.cellWidth / 2) - (size.x / 2);
    pos.y = pos.y + (UI.cellHeight / 2) - (size.y / 2);

    DrawTextEx(UI.font, text, pos, UI.fontSize, 1, cell.color);

    free(text);
}

void DrawTextLeftAligned(Vector2 pos, Cell cell)
{
    char* text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(UI.font, text, UI.fontSize, 1);

    pos.x = pos.x + UI.fontSize / 2;
    pos.y = pos.y + (UI.cellHeight / 2) - (size.y / 2);

    DrawTextEx(UI.font, text, pos, UI.fontSize, 1, cell.color);

    free(text);
}

void DrawTextAligned(Vector2 pos, Cell cell, size_t cellIndex)
{
    switch (cell.alignment) {
    case ALIGN_LEFT:
        DrawTextLeftAligned(CalcTextPos(pos, cellIndex), cell);
        break;
    case ALIGN_CENTER:
        DrawTextCentered(CalcTextPos(pos, cellIndex), cell);
        break;
    case ALIGN_RIGHT:
        assert(!"TODO: ALIGN_RIGHT");
        break;
    default:
        assert(!"Value of cell.alignment is not valid!\n");
        exit(-1);
    }
}

void DrawTextHighlight()
{
    Vector2 pos = {0};
    pos = CalcTextPos(pos, Sheet.index);

    // char* cellText = gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH);
    // float cellTextSpan = MeasureTextEx(UI.font, cellText, UI.fontSize, 1).x;

    // size_t selectionLen = Sheet.selection.end - Sheet.selection.start;
    // char *selectedText = malloc(selectionLen);
    // memset(selectedText, 0, selectionLen);
    // strncpy(selectedText, cellText + Sheet.selection.start, selectionLen + 1);
    // float selectionSpan = MeasureTextEx(UI.font, selectedText, UI.fontSize, 1).x;

    // // char* prefix = gapStrToStr(Sheet.cell->gapStr, Sheet.selection.start);
    // float offset = cellTextSpan - selectionSpan;

    // --------------------------------------------------------------------------------------------------------------- //
    size_t selectionLen = Sheet.selection.end - Sheet.selection.start;
    char *selectedText = gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH) + Sheet.selection.start;
    selectedText[selectionLen] = 0;
    float selectionSpan = MeasureTextEx(UI.font, selectedText, UI.fontSize, 1).x;

    float cellTextSpan = MeasureTextEx(UI.font, gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH), UI.fontSize, 1).x;
    float offset = MeasureTextEx(UI.font, gapStrToStr(Sheet.cell->gapStr, Sheet.selection.start), UI.fontSize, 1).x;
    // --------------------------------------------------------------------------------------------------------------- //

    pos.x += (UI.cellWidth / 2) - (cellTextSpan / 2) + offset;
    DrawRectangleRec((Rectangle){pos.x, pos.y, selectionSpan + 2, UI.cellHeight}, COLOR_HIGHLIGHT);

    // free(cellText);
    free(selectedText);
}

#endif