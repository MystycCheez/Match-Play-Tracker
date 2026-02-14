#ifndef DRAW_H
#define DRAW_H

#include "raylib.h"
#include "structs.h"

void DrawSelectionBorders();
void DrawCursor();
void DrawTextCentered(Vector2 pos, Cell cell);
void DrawTextLeftAligned(Vector2 pos, Cell cell);
void DrawTextAligned(Cell cell, size_t cellIndex);
void DrawTextHighlight();

#endif