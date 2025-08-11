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

void MouseSheetHandler(CollisionMap Collision, Cell *sheet, size_t *cellIndex)
{
    if (!Mouse.pressed) return;
    if (Collision.sheet) {
        if (xyToIndex(Mouse.pos) == *cellIndex) {
            GVARS.scope = SCOPE_CELL;
        }
        else {
            *cellIndex = xyToIndex(Mouse.pos);
            GVARS.scope = sheet[*cellIndex].selectable ? SCOPE_SHEET : SCOPE_OVERVIEW;
            cellIndex = GVARS.scope > SCOPE_OVERVIEW ? cellIndex : 0;
        }
    }
    else {
        GVARS.scope = SCOPE_OVERVIEW;
        cellIndex = 0;
    }
}

void MouseHandler(Cell *sheet, size_t *cellIndex)
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
    MouseSheetHandler(Collision, sheet, cellIndex);
}

void EnterNavigationHandler(Cell *sheet, size_t *cellIndex)
{
    if (*cellIndex == CELL_COUNT - 5) {
        if (sheet[*cellIndex + 1].gapStr.str[0] != 0) {
            *cellIndex = 0;
            GVARS.scope = SCOPE_OVERVIEW;
        } else *cellIndex = *cellIndex + 1;
        return;
    } else if (*cellIndex == CELL_COUNT - 4) {
        if (sheet[*cellIndex - 1].gapStr.str[0] != 0) {
            *cellIndex = 0;
            GVARS.scope = SCOPE_OVERVIEW;
        } else *cellIndex = *cellIndex - 1;
        return;
    } 
    if (*cellIndex % 3 == 2) {
        if (sheet[*cellIndex - 1].gapStr.str[0] == 0) {
            *cellIndex = *cellIndex - 1;
        } else *cellIndex = *cellIndex + 2;
    } else if (*cellIndex % 3 == 1) {
        if (sheet[*cellIndex + 1].gapStr.str[0] == 0) {
            *cellIndex = *cellIndex + 1;
        } else *cellIndex = *cellIndex + 3;
    }
}

void CellOverwriteHandler(Cell *sheet, size_t cellIndex)
{
    if (cellIndex > 2 && cellIndex < CELL_COUNT - 3) {
        char *filteredText = filterCellText(sheet[cellIndex].gapStr.str);
        OverwriteStr(&sheet[cellIndex].gapStr, filteredText, 0, CELL_TEXT_LENGTH);
    }
}

void CellInputHandler(Cell *sheet, size_t *cellIndex)
{
    if (GVARS.scope == SCOPE_OVERVIEW) return;
    if (*cellIndex == 0) return;
    int key_char = GetCharPressed();

    while (key_char > 0) {
        if ((key_char >= 32) && (key_char <= 125)) {
            if (GVARS.scope == SCOPE_CELL) {
                if (GVARS.selection.exists) {
                    replaceChar(&sheet[*cellIndex].gapStr, (char)key_char);
                    Deselect();
                }
                else placeChar(&sheet[*cellIndex].gapStr, (char)key_char);
            }
            else {
                Deselect();
                GVARS.scope = SCOPE_CELL;
                placeChar(&sheet[*cellIndex].gapStr, (char)key_char);
            }
        }
        key_char = GetCharPressed();
    }
}

void SheetKeyPressHandler(Cell *sheet, size_t *cellIndex)
{
    if (GVARS.scope != SCOPE_SHEET) return;
    if (*cellIndex == 0) return;
    CellInputHandler(sheet, cellIndex);
    if (Key.escape) {
        GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (Key.enter) {
        CellOverwriteHandler(sheet, *cellIndex);
        EnterNavigationHandler(sheet, cellIndex);
        UpdateScores(sheet);
        if (*cellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (Key.delete) {
        OverwriteStr(&sheet[*cellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    }
    if (Key.backspace) {
        OverwriteStr(&sheet[*cellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_CELL;
        return;
    }
    if (Key.left) {
        if (*cellIndex % 3 == 2) {
            *cellIndex = *cellIndex - 1;
        }
    }
    if (Key.right) {
        if (*cellIndex % 3 == 1) {
            *cellIndex = *cellIndex + 1;
        }
    }
    if (Key.down) {
        if (*cellIndex > CELL_COUNT - 6) {
            return;
        }
        *cellIndex = *cellIndex + 3;
    }
    if (Key.up) {
        if (*cellIndex < 3)
            return;
        *cellIndex = *cellIndex - 3;
    }
    if (Key.ctrl) {
        if (Key.v) {
            placeString(&sheet[*cellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
            GVARS.scope = SCOPE_CELL;
            return;
        }
        if (Key.l) {
            if (loadTimes(sheet)) {
                UpdateScores(sheet);
                printf("Loaded times from times/times.txt\n");
            }
        }
        if (Key.s) {
            saveTimes(sheet);
            printf("Saved times to times/times.txt\n");
        }
        if (Key.b) {
            ExportToBBCode(sheet);
            printf("Exported to BBCode\n");
        }
        if (Key.delete) {
            ClearTimes(sheet);
            UpdateScores(sheet);
            printf("Sheet Cleared\n");
        }
    }
}

void OverviewInputHandler(Cell* sheet)
{
    if (GVARS.scope != SCOPE_OVERVIEW) return;

    if (Key.l) {
        if (loadTimes(sheet)) {
            UpdateScores(sheet);
            printf("Loaded times from times/times.txt\n");
        }
    }
    if (Key.s) {
        saveTimes(sheet);
        printf("Saved times to times/times.txt\n");
    }
    if (Key.b) {
        ExportToBBCode(sheet);
        printf("Exported to BBCode\n");
    }
    if (Key.delete) {
        ClearTimes(sheet);
        UpdateScores(sheet);
        printf("Sheet Cleared\n");
    }
}

void CellKeyPressHandler(Cell *sheet, size_t *cellIndex)
{
    if (GVARS.scope != SCOPE_CELL) return;
    CellInputHandler(sheet, cellIndex);
    if (Key.escape) {
        if (GVARS.selection.exists) Deselect();
        else {
            GVARS.scope = SCOPE_SHEET;
            // TODO: Undo action
        }
        return;
    }
    if (Key.enter) {
        CellOverwriteHandler(sheet, *cellIndex);
        EnterNavigationHandler(sheet, cellIndex);
        UpdateScores(sheet);
        if (*cellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        else GVARS.scope = SCOPE_SHEET;
        return;
    }
    if (!Key.shift) { // TODO: Clean this up / make it more concise
        if (Key.ctrl) { // TODO: do it by token
            if (Key.left) {
                MoveCursorToIndex(&sheet[*cellIndex].gapStr, 0);
                Deselect();
                return;
            }
            if (Key.right) {
                MoveCursorToIndex(&sheet[*cellIndex].gapStr, strlen(gapStrToStr(sheet[*cellIndex].gapStr, CELL_TEXT_LENGTH)));
                Deselect();
                return;
            }
            if (Key.c) {
                CopyText(sheet[*cellIndex].gapStr);
                return;
            }
            if (Key.x) {
                CopyText(sheet[*cellIndex].gapStr);
                DeleteSelection(&sheet[*cellIndex].gapStr);
                return;
            }
            if (Key.v) {
                placeString(&sheet[*cellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
                return;
            }
        }
        else if (!Key.ctrl) {
            // Redundant Deselect() here, but I'm not sure if I should always deselect if keypress
            // TODO
            if (Key.left) {
                if (cursorLeft(&sheet[*cellIndex].gapStr)) Deselect();
                Deselect();
                return;
            }
            if (Key.right) {
                if (cursorRight(&sheet[*cellIndex].gapStr)) Deselect();
                Deselect();
                return;
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            // TODO: modify deleteCharAtCursor to account for no char being deleted
            deleteCharAtCursor(&sheet[*cellIndex].gapStr);
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
                selectChar(&sheet[*cellIndex].gapStr, DIR_LEFT);
            if (Key.right)
                selectChar(&sheet[*cellIndex].gapStr, DIR_RIGHT);
        }
    }
}

void InputHandler(Cell *sheet, size_t *cellIndex)
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

    MouseHandler(sheet, cellIndex);
    OverviewInputHandler(sheet);
    SheetKeyPressHandler(sheet, cellIndex);
    CellKeyPressHandler(sheet, cellIndex);
    // GenericKeyPressHandler(key, cellIndex);
}

void CursorHandler(size_t cellIndex)
{
    if (Cursor.type == MOUSE_CURSOR_DEFAULT) {
            if (GVARS.scope == SCOPE_CELL) {
                if (CheckCollisionPointRec(Mouse.pos, getCellRect(cellIndex))) {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    Cursor.type = MOUSE_CURSOR_IBEAM;
                }
            }
        } else if (
            (GVARS.scope != SCOPE_CELL) || 
            (!CheckCollisionPointRec(Mouse.pos, getCellRect(cellIndex)))
        ) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            Cursor.type = MOUSE_CURSOR_DEFAULT;
        } 
}

#endif