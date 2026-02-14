#ifndef GET_H
#define GET_H

#include <stdlib.h>

#include "raylib.h"

#include "enums.h"
#include "key.h"

Color getStateColor(Button_State state);
Rectangle getButtonRect(int BTN);
Rectangle getSheetRect();
Rectangle getTitleBarRect();
Rectangle getCellRect(size_t cellIndex);
bool getMoveDir();
bool getMouseMoveDir();
int GetKeyComboIndex(int key, int modifier);
char* GetKeyComboText(int index);
int GetModifier();
int GetKeyIndex(int key);
const char* GetKeyText(int key);
int GetRaylibKey(size_t index);
const char* GetModifierText(Modifier modifier);
const char* GetActionText(int index);
const char* GetHumanReadableActionText(int index);

#endif