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

#endif