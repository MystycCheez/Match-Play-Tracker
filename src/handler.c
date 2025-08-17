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
        }
        else buttonLeft = false;
    }
    else {
        Drag.titleBar = Drag.minimize = false;
    }

    if (Mouse.down && !windowDrag && !(Drag.titleBar || Drag.minimize)) {
        if (Collision.titleBar && !(Collision.exit || Collision.minimize)) {
            windowDrag = true;
            dragOffset = Mouse.pos;
        }
        else windowDrag = false;
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
        if (xyToIndex(Mouse.pos) == GVARS.selectedCellIndex) {
            GVARS.scope = SCOPE_CELL;
        }
        else {
            CellOverwriteHandler();
            UpdateScores();
            GVARS.selectedCellIndex = xyToIndex(Mouse.pos);
            GVARS.scope = sheet[GVARS.selectedCellIndex].selectable ? SCOPE_SHEET : SCOPE_OVERVIEW;
            GVARS.selectedCellIndex = GVARS.scope > SCOPE_OVERVIEW ? GVARS.selectedCellIndex : 0;
        }
    } else {
        GVARS.scope = SCOPE_OVERVIEW;
        GVARS.selectedCellIndex = 0;
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
        CheckCollisionPointRec(Mouse.pos, (Rectangle){0, 0, Window.Width, TOP_BAR_HEIGHT}),
        CheckCollisionPointRec(Mouse.pos, 
            (Rectangle){
                UI.cellWidth, 
                TOP_BAR_HEIGHT, 
                UI.cellWidth * 2, 
                Window.Height - TOP_BAR_HEIGHT
            })
    };
    MouseTitleBarHandler(Collision, windowPos);
    MouseSheetHandler(Collision);
}

// Holy shit this is an eyesore
void EnterNavigationHandler()
{
    if (GVARS.selectedCellIndex == CELL_COUNT - 5) {
        if (sheet[GVARS.selectedCellIndex + 1].gapStr.str[0] != 0) {
            GVARS.selectedCellIndex = 0;
            GVARS.scope = SCOPE_OVERVIEW;
        } else GVARS.selectedCellIndex = GVARS.selectedCellIndex + 1;
        return;
    } else if (GVARS.selectedCellIndex == CELL_COUNT - 4) {
        if (sheet[GVARS.selectedCellIndex - 1].gapStr.str[0] != 0) {
            GVARS.selectedCellIndex = 0;
            GVARS.scope = SCOPE_OVERVIEW;
        } else GVARS.selectedCellIndex = GVARS.selectedCellIndex - 1;
        return;
    } 
    if (GVARS.selectedCellIndex % 3 == 2) {
        if (sheet[GVARS.selectedCellIndex - 1].gapStr.str[0] == 0) {
            GVARS.selectedCellIndex = GVARS.selectedCellIndex - 1;
        } else GVARS.selectedCellIndex = GVARS.selectedCellIndex + 2;
    } else if (GVARS.selectedCellIndex % 3 == 1) {
        if (sheet[GVARS.selectedCellIndex + 1].gapStr.str[0] == 0) {
            GVARS.selectedCellIndex = GVARS.selectedCellIndex + 1;
        } else GVARS.selectedCellIndex = GVARS.selectedCellIndex + 3;
    }
}

void CellOverwriteHandler()
{
    if (GVARS.selectedCellIndex > 2 && GVARS.selectedCellIndex < CELL_COUNT - 3) {
        char *filteredText = filterCellText(sheet[GVARS.selectedCellIndex].gapStr.str);
        OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, filteredText, 0, CELL_TEXT_LENGTH);
    }
}

void CellInputHandler()
{
    if (GVARS.scope == SCOPE_OVERVIEW) return;
    if (GVARS.selectedCellIndex == 0) return;
    int key_char = GetCharPressed();

    while (key_char > 0) {
        if ((key_char >= 32) && (key_char <= 125)) {
            if (GVARS.scope == SCOPE_CELL) {
                if (GVARS.selection.exists) {
                    DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
                    placeChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
                    Deselect();
                } else placeChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
            } else {
                Deselect();
                GVARS.scope = SCOPE_CELL;
                OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
                CellOverwriteHandler();
                UpdateScores();
                placeChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
            }
        }
        key_char = GetCharPressed();
    }
}


void InputHandler()
{
    CellInputHandler();
    KeyData.pressed = GetKeyPressed();
    KeyData.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    KeyData.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    KeyData.alt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);

    Action_Function[ActionTable[GVARS.scope][GetKeyComboIndex(GetKeyIndex(KeyData.pressed), GetModifier())]]();

    MouseHandler();
}

void CursorHandler()
{
    if (Cursor.type == MOUSE_CURSOR_DEFAULT) {
            if (GVARS.scope == SCOPE_CELL) {
                if (CheckCollisionPointRec(Mouse.pos, getCellRect(GVARS.selectedCellIndex))) {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    Cursor.type = MOUSE_CURSOR_IBEAM;
                }
            }
        } else if (
            (GVARS.scope != SCOPE_CELL) || 
            (!CheckCollisionPointRec(Mouse.pos, getCellRect(GVARS.selectedCellIndex)))
        ) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            Cursor.type = MOUSE_CURSOR_DEFAULT;
        } 
}

#endif