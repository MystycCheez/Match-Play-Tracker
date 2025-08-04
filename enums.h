#ifndef ENUMS_H
#define ENUMS_H

#include "includes.h"

typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_RIGHT = 1, // Not Currently used
    ALIGN_CENTER = 2,
} Alignment;

typedef enum {
    TEXTURE_UNHIGHLIGHTED = 0,
    TEXTURE_HIGHLIGHTED = 1,
    TEXTURE_SELECTED = 2,
} Texture_Mode;

typedef enum {
    STATE_BTN_UNHIGHLIGHTED = 0,
    STATE_BTN_HIGHLIGHTED = 1,
    STATE_BTN_PRESSED = 2,
} State_Button;

typedef enum {
    BTN_EXIT = 0,
    BTN_MINIMIZE = 1,
} Button_List;

typedef enum {
    COLLISION_EXIT = 0,
    COLLISION_MINIMIZE = 1,
    COLLISION_TOP_BAR = 2,
    COLLISION_SHEET = 3,
} Collision_List;

typedef enum {
    TEXT_NA = -1,
    TEXT_VETO = 0,
    TEXT_DNF = 1,
} Text_Type;

typedef enum {
    LEVELS_GE = 0,
    LEVELS_PD = 1,
} Game_Levels;

typedef enum {
    SCOPE_OVERVIEW = 0,
    SCOPE_SHEET = 1,
    SCOPE_CELL = 2,
} Scope_Level;

#endif