#include "headers.h"

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