#ifndef HANDLER_C
#define HANDLER_C

#include "headers.h"

// TODO: Add save reminder dialog
void ExitHandler()
{
    GVARS.shouldExit = true;
}

void MouseTitleBarHandler(CollisionMap Collision, Vector2 windowPos)
{
    static bool buttonLeft = false;
    static bool windowDrag = false;
    static CollisionMap Drag = {false};
    static Vector2 dragOffset = {0};

    for (size_t i = 0; i < 2; i++) {
        UI.buttons[i].state = ((bool *)(&Collision))[i] ? STATE_BTN_HIGHLIGHTED : STATE_BTN_UNHIGHLIGHTED;
    }
    if (Mouse.down) {
        if (Collision.exit && !Drag.titleBar) {
            Drag.titleBar = true;
            UI.buttons[BTN_EXIT].state = STATE_BTN_PRESSED;
        }
        if (Collision.minimize && !Drag.minimize) {
            Drag.minimize = true;
            UI.buttons[BTN_MINIMIZE].state = STATE_BTN_PRESSED;
        }
        if (!(Collision.exit || Collision.minimize) && (Drag.titleBar || Drag.minimize)) {
            buttonLeft = true;
        } else buttonLeft = false;
    } else {
        Drag.titleBar = Drag.minimize = false;
    }

    if (Mouse.down && !windowDrag && !(Drag.titleBar || Drag.minimize)) {
        if (Collision.titleBar && !(Collision.exit || Collision.minimize)) {
            windowDrag = true;
            dragOffset = Mouse.pos;
        } else windowDrag = false;
    }
    if (windowDrag) {
        windowPos.x += Mouse.pos.x - dragOffset.x;
        windowPos.y += Mouse.pos.y - dragOffset.y;
        SetWindowPosition(windowPos.x, windowPos.y);
        if (!Mouse.down) windowDrag = false;
    }
    if (Mouse.released && !windowDrag && !(Drag.titleBar || Drag.minimize) && !buttonLeft) {
        if (Collision.exit) {
            UI.buttons[BTN_EXIT].state = STATE_BTN_PRESSED;
            ExitHandler();
        }
        if (Collision.minimize) {
            UI.buttons[BTN_MINIMIZE].state = STATE_BTN_PRESSED;
            MinimizeWindow();
        }
    }
}

void MouseSheetHandler(CollisionMap Collision)
{
    if (!Mouse.pressed) return;
    if (Collision.sheet) {
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
    } else {
        unselectCells();
    }
}

void MouseHandler()
{
    Mouse.pos = GetMousePosition();
    Vector2 windowPos = GetWindowPosition();

    Mouse.down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    Mouse.pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Mouse.released = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

    CollisionMap Collision = {
        CheckCollisionPointRec(Mouse.pos, getButtonRect(UI.buttons[BTN_EXIT])),
        CheckCollisionPointRec(Mouse.pos, getButtonRect(UI.buttons[BTN_MINIMIZE])),
        CheckCollisionPointRec(Mouse.pos, (Rectangle){0, 0, Window.Width, UI.topBarHeight}),
        CheckCollisionPointRec(Mouse.pos, 
            (Rectangle){
                0, 
                UI.topBarHeight, 
                UI.cellWidth * 3, 
                UI.cellHeight * ROWS
            })
    };
    MouseTitleBarHandler(Collision, windowPos);
    MouseSheetHandler(Collision);
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
// GetCharPressed() handles holding down a key to repeated an input
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