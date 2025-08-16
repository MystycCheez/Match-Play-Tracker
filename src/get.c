#ifndef GET_C
#define GET_C

#include "headers.h"

Color getStateColor(Button_State state)
{
    if (state == STATE_BTN_PRESSED)
        return GRAY;
    if (state == STATE_BTN_HIGHLIGHTED)
        return LIGHTGRAY;
    return WHITE;
}

Rectangle getButtonRect(Button button)
{
    return (Rectangle){button.pos.x, button.pos.y, button.size.x, button.size.y};
}

Rectangle getCellRect(size_t cellIndex)
{
    Vector2 pos = indexToXY(cellIndex);
    return (Rectangle){pos.x, pos.y, UI.cellWidth, UI.cellHeight};
}

bool getMoveDir()
{
    if (KeyData.pressed == KEY_LEFT) return DIR_LEFT;
    if (KeyData.pressed == KEY_RIGHT) return DIR_RIGHT;
    assert(!"Shouldn't be trying to get a direction when not pressing left or right!");
}

int GetKeyComboIndex(int key, int modifier)
{
    if (key == -1) return UNUSED_KEY_COMBO;
    return key + (modifier * KEY_COUNT);
}

char* GetKeyComboText(int index)
{
    size_t size = 50;
    size_t pos = 0;
    char* buffer = calloc(size, 1);
    for (size_t i = 1; i < 5; i++) {
        if ((size_t)(index / KEY_COUNT) == i) {
            pos += snprintf(buffer, size - pos, "%s + ", GetModifierText(i));
            break;
        }
    }
    index = (index % KEY_COUNT);
    const char* tmp = GetKeyText(index);
    snprintf(buffer + pos, pos - size, tmp);
    return buffer;
}

int GetModifier()
{
    int modifier = 0;
    if (KeyData.ctrl) modifier += M_CTRL;
    if (KeyData.alt) modifier += M_ALT;
    if (KeyData.shift) modifier += M_SHIFT;
    return modifier;
}

int GetKeyIndex(int key)
{
    for (size_t i = 0; i < KEY_COUNT; i++) {
        if (GetRaylibKey(i) == key) return i;
    }
    return -1;
}

const char* GetKeyText(int key)
{
    switch (key) {
        case K_LEFT: return "LEFT";
        case K_RIGHT: return "RIGHT";
        case K_UP: return "UP";
        case K_DOWN: return "DOWN";
        case K_ENTER: return "ENTER";
        case K_ESCAPE: return "ESCAPE";
        case K_DELETE: return "DELETE";
        case K_BACKSPACE: return "BACKSPACE";
        case K_TAB: return "TAB";
        case K_HOME: return "HOME";
        case K_END: return "END";
        case K_A: return "A";
        case K_C: return "C";
        case K_D: return "D";
        case K_X: return "X";
        case K_V: return "V";
        case K_L: return "L";
        case K_S: return "S";
        case K_B: return "B";
        default: assert(!"Not a valid key!\n");
    }
}

int GetRaylibKey(size_t index)
{
    switch (index) {
        case K_LEFT: return KEY_LEFT;
        case K_RIGHT: return KEY_RIGHT;
        case K_UP: return KEY_UP;
        case K_DOWN: return KEY_DOWN;
        case K_ENTER: return KEY_ENTER;
        case K_ESCAPE: return KEY_ESCAPE;
        case K_DELETE: return KEY_DELETE;
        case K_BACKSPACE: return KEY_BACKSPACE;
        case K_TAB: return KEY_TAB;
        case K_HOME: return KEY_HOME;
        case K_END: return KEY_END;
        case K_A: return KEY_A;
        case K_C: return KEY_C;
        case K_D: return KEY_D;
        case K_X: return KEY_X;
        case K_V: return KEY_V;
        case K_L: return KEY_L;
        case K_S: return KEY_S;
        case K_B: return KEY_B;
        default: assert(!"Not a valid key!\n");
    }
}

const char* GetModifierText(Modifier modifier)
{
    switch (modifier) {
        case M_NONE: return "\0";
        case M_CTRL: return "Ctrl";
        case M_ALT: return "Alt";
        case M_SHIFT: return "Shift";
        case M_CTRL_ALT: return "Ctrl + Alt";
        case M_CTRL_SHIFT: return "Ctrl + Shift";
        case M_ALT_SHIFT: return "Alt + Shift";
        default: assert(!"Not a valid modifier!\n");
    }
}

const char* GetActionText(int index)
{
    switch (index) {
        case A_DONOTHING:                   return "A_DONOTHING";
        case A_SCOPEDECREASE:               return "A_SCOPEDECREASE";
        case A_SELECTCHAR:                  return "A_SELECTCHAR";
        case A_SELECTALL:                   return "A_SELECTALL";
        case A_SELECTALLATCURSORTOWARDSDIR: return "A_SELECTALLATCURSORTOWARDSDIR";
        case A_MOVECURSOR:                  return "A_MOVECURSOR";
        case A_MOVECURSORBYTOKEN:           return "A_MOVECURSORBYTOKEN";
        case A_MOVECURSORTOSTART:           return "A_MOVECURSORTOSTART";
        case A_MOVECURSORTOEND:             return "A_MOVECURSORTOEND";
        case A_LOADTIMES:                   return "A_LOADTIMES";
        case A_SAVETIMES:                   return "A_SAVETIMES";
        case A_EXPORTTIMES:                 return "A_EXPORTTIMES";
        case A_CLEARTIMES:                  return "A_CLEARTIMES";
        case A_DESELECT_OR_UNDO_BACKOUT:    return "A_DESELECT_OR_UNDO_BACKOUT";
        case A_OVERWRITE_UPDATESCORE:       return "A_OVERWRITE_UPDATESCORE";
        case A_NAVIGATETONEXTCELL:          return "A_NAVIGATETONEXTCELL";
        case A_COPY:                        return "A_COPY";
        case A_CUT:                         return "A_CUT";
        case A_PASTE:                       return "A_PASTE";
        case A_DELETE:                      return "A_DELETE";
        case A_DELETECELLTEXT:              return "A_DELETECELLTEXT";
        case A_DELETECELLTEXTANDENTERINTO:  return "A_DELETECELLTEXTANDENTERINTO";
        case A_DELETESELECTION:             return "A_DELETESELECTION";
        case A_DELETECHAR:                  return "A_DELETECHAR";
        case A_NAVIGATELEFT:                return "A_NAVIGATELEFT";
        case A_NAVIGATERIGHT:               return "A_NAVIGATERIGHT";
        case A_NAVIGATEUP:                  return "A_NAVIGATEUP";
        case A_NAVIGATEDOWN:                return "A_NAVIGATEDOWN";
        case A_SWAPVETOCOLOR:               return "A_SWAPVETOCOLOR";
        default: assert(!"Not a valid action!\n");
    }
}

const char* GetHumanReadableActionText(int index)
{
    switch (index) {
        case A_DONOTHING:                   return "N/A";
        case A_SCOPEDECREASE:               return "Scope decrease";
        case A_SELECTCHAR:                  return "Select char";
        case A_SELECTALL:                   return "Select all cell text";
        case A_SELECTALLATCURSORTOWARDSDIR: return "Select from cursor by dir";
        case A_MOVECURSOR:                  return "Move cursor";
        case A_MOVECURSORBYTOKEN:           return "Move cursor by token";
        case A_MOVECURSORTOSTART:           return "Move cursor to start";
        case A_MOVECURSORTOEND:             return "Move cursor to end";
        case A_LOADTIMES:                   return "Load times";
        case A_SAVETIMES:                   return "Save times";
        case A_EXPORTTIMES:                 return "Export times";
        case A_CLEARTIMES:                  return "Clear times";
        case A_DESELECT_OR_UNDO_BACKOUT:    return "Deselect or undo and backout";
        case A_OVERWRITE_UPDATESCORE:       return "Overwrite and update score";
        case A_NAVIGATETONEXTCELL:          return "Navigate to next cell";
        case A_COPY:                        return "Copy";
        case A_CUT:                         return "Cut";
        case A_PASTE:                       return "Paste";
        case A_DELETE:                      return "Delete";
        case A_DELETECELLTEXT:              return "Delete cell text";
        case A_DELETECELLTEXTANDENTERINTO:  return "Delete cell text; enter cell";
        case A_DELETESELECTION:             return "Delete selection";
        case A_DELETECHAR:                  return "Delete char";
        case A_NAVIGATELEFT:                return "Navigate left";
        case A_NAVIGATERIGHT:               return "Navigate right";
        case A_NAVIGATEUP:                  return "Navigate up";
        case A_NAVIGATEDOWN:                return "Navigate down";
        case A_SWAPVETOCOLOR:               return "Swap veto text color";
        default: assert(!"Not a valid action!\n");
    }
}

#endif