#ifndef HANDLER_C
#define HANDLER_C

#include "headers.h"

// TODO: Add save reminder dialog
void ExitHandler()
{
    GVARS.shouldExit = true;
}

void MouseHoverHandler()
{
    for (int i = 0; i < BTN_COUNT; i++) {
        if (i == Mouse.current) {
            UI.buttons[i].state = STATE_BTN_HIGHLIGHTED;
        } else UI.buttons[i].state = STATE_BTN_UNHIGHLIGHTED;
    }
}

void WindowDragHandler()
{
    Vector2 windowPos = GetWindowPosition();
    windowPos.x += Mouse.pos.x - Mouse.startDragPos.x;
    windowPos.y += Mouse.pos.y - Mouse.startDragPos.y;
    SetWindowPosition(windowPos.x, windowPos.y);
}

void TextDragHandler()
{
    size_t index = xToCursorIndex(Mouse.pos.x);
    SelectToIndex(&Sheet.cell->gapStr, getMouseMoveDir(), index);
}

void MouseDragHandler()
{
    if (Mouse.startDrag == TITLEBAR) {
        WindowDragHandler();
    } else if (Mouse.startDrag == SHEET) {
        if (GVARS.scope == SCOPE_SHEET) {
            // TODO: Dragging cells
        } else {
            if (Mouse.pos.x != Mouse.startDragPos.x)
            TextDragHandler();
        }
    } 
}

void MouseSheetHandler()
{
    size_t index = xyToIndex(Mouse.pos);
    if (Sheet.cellList[index].selectable) {
        if (index == Sheet.index) {
            GVARS.scope = SCOPE_CELL;
        } else {
            GVARS.scope = SCOPE_SHEET;
            Deselect();
            if ((Sheet.index % 3 != 0) && (Sheet.index > 3)) {
                char* cellText = gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH);
                if (strlen(cellText) > 0) {
                    CellOverwriteHandler();
                    UpdateScores();
                }
                free(cellText);
            }
            updateSheetIndex(index);
        }
    } else unselectCells();
}

void MouseReleaseHandler()
{
    if (Mouse.current == EXIT) {
        UI.buttons[BTN_EXIT].state = STATE_BTN_PRESSED;
        ExitHandler();
    } else if (Mouse.current == MINIMIZE) {
        UI.buttons[BTN_MINIMIZE].state = STATE_BTN_PRESSED;
        MinimizeWindow();
    } else if (Mouse.current == TITLEBAR) {
        A_Deselect_Or_Undo_Backout();
        unselectCells();
    } else if (Mouse.current == SHEET) {
        // TODO: Multiple cell selection?
    }
}

void MouseCollisionHandler()
{
    if (Mouse.pressed) {
        if (Mouse.current == SHEET) {
            MouseSheetHandler();
        }
        Mouse.startDrag = Mouse.current;
        Mouse.startDragPos = Mouse.pos;
        if (GVARS.scope == SCOPE_CELL) {
            size_t start_index = xToCursorIndex(Mouse.pos.x);
            MoveCursorToIndex(&Sheet.cell->gapStr, start_index);
        }
    } else if (Mouse.down && !Mouse.pressed) {
        updateCollisionMap();
        updateCollider();
        MouseDragHandler();
    } else if (Mouse.released) {
        if (Mouse.current == Mouse.startDrag) {
            MouseReleaseHandler();
        }
        Mouse.startDrag = Mouse.current;
        Mouse.startDragPos = Mouse.pos;
    } else {
        updateCollider();
        MouseHoverHandler();
    }
}

void updateCollider()
{
    if      (Mouse.Collision.exit)     {Mouse.current = EXIT;}
    else if (Mouse.Collision.minimize) {Mouse.current = MINIMIZE;}
    else if (Mouse.Collision.titleBar) {Mouse.current = TITLEBAR;}
    else                               {Mouse.current = SHEET;}
}

void updateCollisionMap()
{
    Mouse.Collision = (CollisionMap) {
        CheckCollisionPointRec(Mouse.pos, getButtonRect(BTN_EXIT)),
        CheckCollisionPointRec(Mouse.pos, getButtonRect(BTN_MINIMIZE)),
        CheckCollisionPointRec(Mouse.pos, TitleBar.Rec),
        CheckCollisionPointRec(Mouse.pos, getSheetRect())
    };
}

void MouseHandler()
{
    Mouse.pos = GetMousePosition();
    Mouse.down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    Mouse.pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Mouse.released = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

    updateCollisionMap();
    MouseCollisionHandler();
}

void EnterNavigationHandler()
{
    if (Sheet.index % 3 == 0) {
        if (Sheet.index + 3 == CELL_COUNT - 3) {
            unselectCells();
        } else updateSheetIndex(Sheet.index + 3);
        return;
    }
    if (Sheet.index == CELL_COUNT - 5) {
        if (getOppositeCell().gapStr.str[0] != 0) {
            unselectCells();
        } else updateSheetIndex(getOppositeCellIndex());
        return;
    } else if (Sheet.index == CELL_COUNT - 4) {
        if (getOppositeCell().gapStr.str[0] != 0) {
            unselectCells();
        } else updateSheetIndex(getOppositeCellIndex());
        return;
    } 
    if (Sheet.index % 3 == 2) {
        if (getOppositeCell().gapStr.str[0] == 0) {
            updateSheetIndex(getOppositeCellIndex());
        } else updateSheetIndex(Sheet.index + 2);
    } else if (Sheet.index % 3 == 1) {
        if (getOppositeCell().gapStr.str[0] == 0) {
            updateSheetIndex(getOppositeCellIndex());
        } else updateSheetIndex(Sheet.index + 3);
    }
}

void CellOverwriteHandler()
{
    char* cellText = gapStrToStr(Sheet.cell->gapStr, CELL_TEXT_LENGTH);
    if (cellText[0] == '\0') {
        free(cellText);
        return;
    }
    if (cellText[0] == '#') {
        setCellTextColor(cellText);
    }
    if (Sheet.index > 2 && Sheet.index < CELL_COUNT - 3) {
        char *filteredText = filterCellText(Sheet.cell->gapStr.str);
        OverwriteStr(&Sheet.cell->gapStr, filteredText, 0, CELL_TEXT_LENGTH);
        free(filteredText);
    }
    free (cellText);
}

void CellInputHandler()
{
    if (GVARS.scope == SCOPE_OVERVIEW) return;
    if (Sheet.index == 0) return;
    char key_char = 0;

    while ((key_char = GetCharPressed()) > 0) {
        if ((key_char >= 32) && (key_char <= 125)) {
            if (GVARS.scope == SCOPE_CELL) {
                if (Sheet.selection.exists) {
                    DeleteSelection(&Sheet.cell->gapStr);
                    placeChar(&Sheet.cell->gapStr, key_char);
                    Deselect();
                } else placeChar(&Sheet.cell->gapStr, key_char);
            } else {
                GVARS.scope = SCOPE_CELL;
                OverwriteStr(&Sheet.cell->gapStr, "\0", 0, CELL_TEXT_LENGTH);
                CellOverwriteHandler();
                UpdateScores();
                placeChar(&Sheet.cell->gapStr, key_char);
            }
        }
    }
}

// GetKeyPressed() does not act the same as GetCharPressed()
// GetCharPressed() handles holding down a key to repeat an input
// GetKeyPressed() does not do this!
void KeyHandler()
{
    while ((KeyData.pressed = GetKeyPressed()) > 0) {
        KeyData.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
        KeyData.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        KeyData.alt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
        
        int KeyIndex = GetKeyIndex(KeyData.pressed);
        if (KeyIndex == -1) break;
        int KeyCombo = GetKeyComboIndex(KeyIndex, GetModifier());
        if (KeyCombo == UNUSED_KEY_COMBO) break;
        Action CurrentAction = ActionTable[GVARS.scope][KeyCombo];
        if (CurrentAction == A_DONOTHING) break;
        Action_Function[CurrentAction]();
        
        printf("%s\n", GetActionText(CurrentAction));
    }
}

void CursorHandler()
{
    if (Cursor.type == MOUSE_CURSOR_DEFAULT) {
        if (GVARS.scope == SCOPE_CELL) {
            if (CheckCollisionPointRec(Mouse.pos, getCellRect(Sheet.index))) {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                Cursor.type = MOUSE_CURSOR_IBEAM;
            }
        }
    } else if (
        (GVARS.scope != SCOPE_CELL) || 
        (!CheckCollisionPointRec(Mouse.pos, getCellRect(Sheet.index)))
    ) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        Cursor.type = MOUSE_CURSOR_DEFAULT;
    } 
}

#endif