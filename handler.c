#ifndef HANDLER_C
#define HANDLER_C

#include "includes.h"

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
            GVARS.selectedCellIndex = xyToIndex(Mouse.pos);
            GVARS.scope = sheet[GVARS.selectedCellIndex].selectable ? SCOPE_SHEET : SCOPE_OVERVIEW;
            GVARS.selectedCellIndex = GVARS.scope > SCOPE_OVERVIEW ? GVARS.selectedCellIndex : 0;
        }
    }
    else {
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
                    replaceChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
                    Deselect();
                }
                else placeChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
            }
            else {
                Deselect();
                GVARS.scope = SCOPE_CELL;
                placeChar(&sheet[GVARS.selectedCellIndex].gapStr, (char)key_char);
            }
        }
        key_char = GetCharPressed();
    }
}

void SheetKeyPressHandler()
{
    if (GVARS.scope != SCOPE_SHEET) return;
    if (GVARS.selectedCellIndex == 0) return;
    CellInputHandler();
    if (Key.escape) {
        GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (Key.enter) {
        CellOverwriteHandler();
        EnterNavigationHandler();
        UpdateScores();
        if (GVARS.selectedCellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (Key.delete) {
        OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    }
    if (Key.backspace) {
        OverwriteStr(&sheet[GVARS.selectedCellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_CELL;
        return;
    }
    if (Key.left) {
        if (GVARS.selectedCellIndex % 3 == 2) {
            GVARS.selectedCellIndex = GVARS.selectedCellIndex - 1;
        }
    }
    if (Key.right) {
        if (GVARS.selectedCellIndex % 3 == 1) {
            GVARS.selectedCellIndex = GVARS.selectedCellIndex + 1;
        }
    }
    if (Key.down) {
        if (GVARS.selectedCellIndex > CELL_COUNT - 6) {
            return;
        }
        GVARS.selectedCellIndex = GVARS.selectedCellIndex + 3;
    }
    if (Key.up) {
        if (GVARS.selectedCellIndex < 3)
            return;
        GVARS.selectedCellIndex = GVARS.selectedCellIndex - 3;
    }
    if (Key.ctrl) {
        if (Key.v) {
            placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
            GVARS.scope = SCOPE_CELL;
            return;
        }
        if (Key.l) {
            if (loadTimes()) {
                UpdateScores();
                printf("Loaded times from times/times.txt\n");
            }
        }
        if (Key.s) {
            saveTimes();
            printf("Saved times to times/times.txt\n");
        }
        if (Key.b) {
            ExportToBBCode();
            printf("Exported to BBCode\n");
        }
        if (Key.delete) {
            ClearTimes();
            UpdateScores();
            printf("Sheet Cleared\n");
        }
    }
}

void OverviewInputHandler()
{
    if (GVARS.scope != SCOPE_OVERVIEW) return;

    if (Key.l) {
        if (loadTimes()) {
            UpdateScores();
            printf("Loaded times from times/times.txt\n");
        }
    }
    if (Key.s) {
        saveTimes();
        printf("Saved times to times/times.txt\n");
    }
    if (Key.b) {
        ExportToBBCode();
        printf("Exported to BBCode\n");
    }
    if (Key.delete) {
        ClearTimes();
        UpdateScores();
        printf("Sheet Cleared\n");
    }
}

void CellKeyPressHandler()
{
    if (GVARS.scope != SCOPE_CELL) return;
    CellInputHandler();
    if (Key.escape) {
        if (GVARS.selection.exists) Deselect();
        else {
            GVARS.scope = SCOPE_SHEET;
            // TODO: Undo action
        }
        return;
    }
    if (Key.enter) {
        CellOverwriteHandler();
        EnterNavigationHandler();
        UpdateScores();
        if (GVARS.selectedCellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        else GVARS.scope = SCOPE_SHEET;
        return;
    }
    if (!Key.shift) { // TODO: Clean this up / make it more concise
        if (Key.ctrl) { // TODO: do it by token
            if (Key.left) {
                MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, 0);
                Deselect();
                return;
            }
            if (Key.right) {
                MoveCursorToIndex(&sheet[GVARS.selectedCellIndex].gapStr, strlen(gapStrToStr(sheet[GVARS.selectedCellIndex].gapStr, CELL_TEXT_LENGTH)));
                Deselect();
                return;
            }
            if (Key.c) {
                CopyText(sheet[GVARS.selectedCellIndex].gapStr);
                return;
            }
            if (Key.x) {
                CopyText(sheet[GVARS.selectedCellIndex].gapStr);
                DeleteSelection(&sheet[GVARS.selectedCellIndex].gapStr);
                return;
            }
            if (Key.v) {
                placeString(&sheet[GVARS.selectedCellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
                return;
            }
        }
        else if (!Key.ctrl) {
            // Redundant Deselect() here, but I'm not sure if I should always deselect if keypress
            // TODO
            if (Key.left) {
                if (cursorLeft(&sheet[GVARS.selectedCellIndex].gapStr)) Deselect();
                Deselect();
                return;
            }
            if (Key.right) {
                if (cursorRight(&sheet[GVARS.selectedCellIndex].gapStr)) Deselect();
                Deselect();
                return;
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            // TODO: modify deleteCharAtCursor to account for no char being deleted
            deleteCharAtCursor(&sheet[GVARS.selectedCellIndex].gapStr);
            Deselect();
            return;
        }
    }
    else if (Key.shift) {
        if (Key.ctrl) {
            // if (Key.left) selectLeftToken();
            // if (Key.right) selectRightToken();
        }
        else if (!Key.ctrl) {
            if (Key.left)
                selectChar(&sheet[GVARS.selectedCellIndex].gapStr, DIR_LEFT);
            if (Key.right)
                selectChar(&sheet[GVARS.selectedCellIndex].gapStr, DIR_RIGHT);
        }
    }
}

void InputHandler()
{
    Key.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    Key.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    
    KeyData.pressed = GetKeyPressed();
    KeyData.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    KeyData.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    
    Key.escape = IsKeyPressed(KEY_ESCAPE);
    Key.enter = IsKeyPressed(KEY_ENTER);
    Key.delete = IsKeyPressed(KEY_DELETE);
    Key.backspace = IsKeyPressed(KEY_BACKSPACE);
    
    Key.left = IsKeyPressed(KEY_LEFT);
    Key.right = IsKeyPressed(KEY_RIGHT);
    Key.up = IsKeyPressed(KEY_UP);
    Key.down = IsKeyPressed(KEY_DOWN);
    
    Key.b = IsKeyPressed(KEY_B);
    Key.c = IsKeyPressed(KEY_C);
    Key.l = IsKeyPressed(KEY_L);
    Key.s = IsKeyPressed(KEY_S);
    Key.v = IsKeyPressed(KEY_V);
    Key.x = IsKeyPressed(KEY_X);

    Action[ActionTable[GVARS.scope][GetKeyComboIndex(KeyData.pressed, GetModifierCode())]]();

    MouseHandler();
    OverviewInputHandler();
    SheetKeyPressHandler();
    CellKeyPressHandler();
    // GenericKeyPressHandler(key, cellIndex);
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