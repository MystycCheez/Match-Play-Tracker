#ifndef ENUMS_H
#define ENUMS_H

#include "includes.h"

typedef enum Alignment {
    ALIGN_LEFT = 0,
    ALIGN_RIGHT = 1, // Not Currently used
    ALIGN_CENTER = 2,
} Alignment;

typedef enum Texture_Mode {
    TEXTURE_UNHIGHLIGHTED = 0,
    TEXTURE_HIGHLIGHTED = 1,
    TEXTURE_SELECTED = 2,
} Texture_Mode;

typedef enum Button_State {
    STATE_BTN_UNHIGHLIGHTED = 0,
    STATE_BTN_HIGHLIGHTED = 1,
    STATE_BTN_PRESSED = 2,
} Button_State;

typedef enum Button_List {
    BTN_EXIT = 0,
    BTN_MINIMIZE = 1,
} Button_List;

typedef enum Collision_List {
    COLLISION_EXIT = 0,
    COLLISION_MINIMIZE = 1,
    COLLISION_TOP_BAR = 2,
    COLLISION_SHEET = 3,
} Collision_List;

typedef enum Text_Type {
    TEXT_NA = -1,
    TEXT_VETO = 0,
    TEXT_DNF = 1,
} Text_Type;

typedef enum Game_Levels {
    LEVELS_GE = 0,
    LEVELS_PD = 1,
} Game_Levels;

typedef enum Scope_Level {
    SCOPE_OVERVIEW = 0,
    SCOPE_SHEET = 1,
    SCOPE_CELL = 2,
} Scope_Level;

typedef enum Modifiers {
    M_NONE = 0,
    M_CTRL = 1,
    M_SHIFT = 2,
    M_CTRL_SHIFT = 3,
} Modifiers;

#endif