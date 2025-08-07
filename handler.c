#ifndef HANDLER_C
#define HANDLER_C

#include "includes.h"

// TODO: Add save reminder dialog
void ExitHandler()
{
    GVARS.shouldExit = true;
}

void MouseTitleBarHandler(CollisionMap Collision, MouseState Mouse, Vector2 mousePos, Vector2 windowPos)
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
            dragOffset = mousePos;
        }
        else windowDrag = false;
    }
    if (windowDrag) {
        windowPos.x += mousePos.x - dragOffset.x;
        windowPos.y += mousePos.y - dragOffset.y;
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

void MouseSheetHandler(CollisionMap Collision, MouseState Mouse, Vector2 mousePos, Cell *sheet, size_t *cellIndex)
{
    if (!Mouse.pressed) return;
    if (Collision.sheet) {
        if (xyToIndex(mousePos) == *cellIndex) {
            GVARS.scope = SCOPE_CELL;
        }
        else {
            *cellIndex = xyToIndex(mousePos);
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
    Vector2 mousePos = GetMousePosition();
    Vector2 windowPos = GetWindowPosition();

    MouseState Mouse = {
        IsMouseButtonDown(MOUSE_BUTTON_LEFT),
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
    };
    CollisionMap Collision = {
        CheckCollisionPointRec(mousePos, getButtonRect(UI.buttons[BTN_EXIT])),
        CheckCollisionPointRec(mousePos, getButtonRect(UI.buttons[BTN_MINIMIZE])),
        CheckCollisionPointRec(mousePos, (Rectangle){0, 0, Window.Width, TOP_BAR_HEIGHT}),
        CheckCollisionPointRec(mousePos, (Rectangle){0, TOP_BAR_HEIGHT, Window.Width, SHEET_HEIGHT})
    };
    MouseTitleBarHandler(Collision, Mouse, mousePos, windowPos);
    MouseSheetHandler(Collision, Mouse, mousePos, sheet, cellIndex);
}

// TODO: Fix this for the Egypt row
void EnterNavigationHandler(Cell *sheet, size_t *cellIndex)
{
    if (*cellIndex > CELL_COUNT - 6) {
        *cellIndex = 0;
        GVARS.scope = SCOPE_OVERVIEW;
    } else {
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

void SheetKeyPressHandler(Cell *sheet, KeyMap key, size_t *cellIndex)
{
    if (GVARS.scope != SCOPE_SHEET) return;
    if (*cellIndex == 0) return;
    CellInputHandler(sheet, cellIndex);
    if (key.escape) {
        GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (key.enter) {
        CellOverwriteHandler(sheet, *cellIndex);
        EnterNavigationHandler(sheet, cellIndex);
        UpdateScores(sheet);
        if (*cellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        return;
    }
    if (key.delete) {
        OverwriteStr(&sheet[*cellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
    }
    if (key.backspace) {
        OverwriteStr(&sheet[*cellIndex].gapStr, "\0", 0, CELL_TEXT_LENGTH);
        GVARS.scope = SCOPE_CELL;
        return;
    }
    if (key.left) {
        if (*cellIndex % 3 == 2) {
            *cellIndex = *cellIndex - 1;
        }
    }
    if (key.right) {
        if (*cellIndex % 3 == 1) {
            *cellIndex = *cellIndex + 1;
        }
    }
    if (key.down) {
        if (*cellIndex > CELL_COUNT - 6) {
            return;
        }
        *cellIndex = *cellIndex + 3;
    }
    if (key.up) {
        if (*cellIndex < 3)
            return;
        *cellIndex = *cellIndex - 3;
    }
    if (key.ctrl) {
        if (key.v) {
            placeString(&sheet[*cellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
            GVARS.scope = SCOPE_CELL;
            return;
        }
        if (key.l) {
            if (loadTimes(sheet)) {
                UpdateScores(sheet);
                printf("Loaded times from times/times.txt\n");
            }
        }
        if (key.s) {
            saveTimes(sheet);
            printf("Saved times to times/times.txt\n");
        }
        if (key.b) {
            ExportToBBCode(sheet);
            printf("Exported to BBCode\n");
        }
        if (key.delete) {
            ClearTimes(sheet);
            UpdateScores(sheet);
            printf("Sheet Cleared\n");
        }
    }
}

void OverviewInputHandler(Cell* sheet, KeyMap key)
{
    if (GVARS.scope != SCOPE_OVERVIEW) return;

    if (key.l) {
        if (loadTimes(sheet)) {
            UpdateScores(sheet);
            printf("Loaded times from times/times.txt\n");
        }
    }
    if (key.s) {
        saveTimes(sheet);
        printf("Saved times to times/times.txt\n");
    }
    if (key.b) {
        ExportToBBCode(sheet);
        printf("Exported to BBCode\n");
    }
    if (key.delete) {
        ClearTimes(sheet);
        UpdateScores(sheet);
        printf("Sheet Cleared\n");
    }
}

void CellKeyPressHandler(Cell *sheet, KeyMap key, size_t *cellIndex)
{
    if (GVARS.scope != SCOPE_CELL) return;
    CellInputHandler(sheet, cellIndex);
    if (key.escape) {
        if (GVARS.selection.exists) Deselect();
        else {
            GVARS.scope = SCOPE_SHEET;
            // TODO: Undo action
        }
        return;
    }
    if (key.enter) {
        CellOverwriteHandler(sheet, *cellIndex);
        EnterNavigationHandler(sheet, cellIndex);
        UpdateScores(sheet);
        if (*cellIndex == 0) GVARS.scope = SCOPE_OVERVIEW;
        else GVARS.scope = SCOPE_SHEET;
        return;
    }
    if (!key.shift) { // TODO: Clean this up / make it more concise
        if (key.ctrl) { // TODO: do it by token
            if (key.left) {
                MoveCursorToIndex(&sheet[*cellIndex].gapStr, 0);
                Deselect();
                return;
            }
            if (key.right) {
                MoveCursorToIndex(&sheet[*cellIndex].gapStr, strlen(gapStrToStr(sheet[*cellIndex].gapStr, CELL_TEXT_LENGTH)));
                Deselect();
                return;
            }
            if (key.c) {
                CopyText(sheet[*cellIndex].gapStr);
                return;
            }
            if (key.x) {
                CopyText(sheet[*cellIndex].gapStr);
                DeleteSelection(&sheet[*cellIndex].gapStr);
                return;
            }
            if (key.v) {
                placeString(&sheet[*cellIndex].gapStr, GetClipboardText(), CELL_TEXT_LENGTH);
                return;
            }
        }
        else if (!key.ctrl) {
            // Redundant Deselect() here, but I'm not sure if I should always deselect if keypress
            // TODO
            if (key.left) {
                if (cursorLeft(&sheet[*cellIndex].gapStr)) Deselect();
                Deselect();
                return;
            }
            if (key.right) {
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
    else if (key.shift) {
        if (key.ctrl) {
            // if (key.left) selectLeftToken();
            // if (key.right) selectRightToken();
        }
        else if (!key.ctrl) {
            if (key.left)
                selectChar(&sheet[*cellIndex].gapStr, DIR_LEFT);
            if (key.right)
                selectChar(&sheet[*cellIndex].gapStr, DIR_RIGHT);
        }
    }
}

void InputHandler(Cell *sheet, size_t *cellIndex)
{
    KeyMap key = {false};

    key.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    key.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    key.escape = IsKeyPressed(KEY_ESCAPE);
    key.enter = IsKeyPressed(KEY_ENTER);
    key.delete = IsKeyPressed(KEY_DELETE);
    key.backspace = IsKeyPressed(KEY_BACKSPACE);

    key.left = IsKeyPressed(KEY_LEFT);
    key.right = IsKeyPressed(KEY_RIGHT);
    key.up = IsKeyPressed(KEY_UP);
    key.down = IsKeyPressed(KEY_DOWN);

    key.c = IsKeyPressed(KEY_C);
    key.x = IsKeyPressed(KEY_X);
    key.v = IsKeyPressed(KEY_V);
    key.l = IsKeyPressed(KEY_L);
    key.s = IsKeyPressed(KEY_S);
    key.b = IsKeyPressed(KEY_B);

    MouseHandler(sheet, cellIndex);
    OverviewInputHandler(sheet, key);
    SheetKeyPressHandler(sheet, key, cellIndex);
    CellKeyPressHandler(sheet, key, cellIndex);
    // GenericKeyPressHandler(key, cellIndex);
}

#endif