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

Rectangle getButtonRect(int BTN)
{
    return (Rectangle) {
        UI.buttons[BTN].pos.x, 
        UI.buttons[BTN].pos.y, 
        UI.buttons[BTN].size.x, 
        UI.buttons[BTN].size.y
    };
}

Rectangle getSheetRect()
{
    return (Rectangle) {
        0, 
        UI.topBarHeight, 
        UI.cellWidth * 3, 
        UI.cellHeight * ROWS
    };
}

Rectangle getTitleBarRect()
{
    return (Rectangle){0, 0, Window.Width, UI.topBarHeight};
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
    if (KeyData.pressed == KEY_HOME) return DIR_LEFT;
    if (KeyData.pressed == KEY_END) return DIR_RIGHT;
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
    for (size_t i = 1; i < MODIFIER_KEY_COUNT; i++) {
        if ((size_t)(index / KEY_COUNT) == i) {
            pos += snprintf(buffer, size - pos, "%s + ", GetModifierText(i));
            break;
        }
    }
    index = (index % KEY_COUNT);
    const char* tmp = GetKeyText(index);
    snprintf(buffer + pos, pos - size, "%s", tmp);
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

const char* GetActionEnumName(int index)
{
    switch (index) {
        #define ACTION(a,b,c) case a: return #a;
            ACTION_LIST
        #undef ACTION
        default:
            fprintf(stderr, "Not a valid action: %d!\n", index);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
    }
}

const char* GetActionText(int index)
{
    switch (index) {
        #define ACTION(a,b,c) case a: return #b;
            ACTION_LIST
        #undef ACTION
        default: 
            fprintf(stderr, "Not a valid action: %d!\n", index);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
    }
}

const char* GetHumanReadableActionText(int index)
{
    switch (index) {
        #define ACTION(a,b,c) case a: return c;
            ACTION_LIST
        #undef ACTION
        default: 
            fprintf(stderr, "Not a valid action: %d!\n", index);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
    }
}

int GetKeyIndex(int key)
{
    switch (key) {
        #define KEY(a,b,c) case b: return a;
            KEY_LIST
        #undef KEY
        default: 
            return -1;
    }
}

const char* GetKeyText(int key)
{
    switch (key) {
        case -1:
            return "";
        #define KEY(a,b,c) case a: return c;
            KEY_LIST
        #undef KEY
        default: 
            fprintf(stderr, "Not a valid key: %d!\n", key);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
    }
}

int GetRaylibKey(size_t index)
{
    switch (index) {
        #define KEY(a,b,c) case a: return b;
            KEY_LIST
        #undef KEY
        default: 
            fprintf(stderr, "Not a valid key: %lld!\n", index);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
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
        case M_CTRL_ALT_SHIFT: return "Ctrl + Alt + Shift";
        default: 
            fprintf(stderr, "Not a valid modifier: %d!\n", modifier);
            fprintf(stderr, "%s:%d, %s\n", __FILE__, __LINE__, __func__);
            exit(1);
    }
}

#endif