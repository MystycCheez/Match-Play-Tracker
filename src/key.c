#include "headers.h"

const char* GetKeyText(int key)
{
    switch (key) {
        #define KEY(a,b,c) case a: return c;
            KEY_LIST
        #undef KEY
        default: assert(!"Not a valid key!\n");
    }
}

int GetRaylibKey(size_t index)
{
    switch (index) {
        #define KEY(a,b,c) case a: return b;
            KEY_LIST
        #undef KEY
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