#ifndef ENUMS_H
#define ENUMS_H

typedef enum Alignment {
    ALIGN_LEFT,
    ALIGN_RIGHT, // Not Currently used
    ALIGN_CENTER,
} Alignment;

typedef enum Texture_Mode {
    TEXTURE_UNHIGHLIGHTED,
    TEXTURE_HIGHLIGHTED,
    TEXTURE_SELECTED,
} Texture_Mode;

typedef enum Button_State {
    STATE_BTN_UNHIGHLIGHTED,
    STATE_BTN_HIGHLIGHTED,
    STATE_BTN_PRESSED,
} Button_State;

typedef enum Button_List {
    BTN_EXIT,
    BTN_MINIMIZE,
    BTN_EXPORT,
    
    BTN_COUNT
} Button_List;

typedef enum Text_Type {
    TEXT_NA = -1,
    TEXT_VETO = 0,
    TEXT_DNF = 1,
} Text_Type;

typedef enum Game_Levels {
    LEVELS_GE,
    LEVELS_PD,
} Game_Levels;

typedef enum Scope_Level {
    SCOPE_OVERVIEW,
    SCOPE_SHEET,
    SCOPE_CELL,

    SCOPE_COUNT
} Scope_Level;

typedef enum Collider {
    NONE = -1,
    EXIT,
    MINIMIZE,
    TITLEBAR,
    SHEET,
} Collider;

#endif