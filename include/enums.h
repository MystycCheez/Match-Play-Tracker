#ifndef ENUMS_H
#define ENUMS_H

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

typedef enum Modifier {
    M_NONE = 0,
    M_CTRL = 1,
    M_ALT = 2,
    M_CTRL_ALT = 3,
    M_SHIFT = 4,
    M_CTRL_SHIFT = 5,
    M_ALT_SHIFT = 6,
    M_CTRL_ALT_SHIFT = 7,

    MODIFIER_KEY_COUNT
} Modifier;

typedef enum Key {
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_ENTER,
    K_ESCAPE,
    K_DELETE,
    K_BACKSPACE,
    K_TAB,
    K_HOME,
    K_END,
    K_A,
    K_C,
    K_D,
    K_X,
    K_V,
    K_L,
    K_S,
    K_B,

    KEY_COUNT
} Key;

// List of Actions
typedef enum Actions {
    A_DONOTHING,
    A_SCOPEDECREASE,
    A_SELECTCHAR,
    A_SELECTALL,
    A_SELECTALLATCURSORTOWARDSDIR,
    A_MOVECURSOR,
    A_MOVECURSORBYTOKEN,
    A_MOVECURSORTOSTART,
    A_MOVECURSORTOEND,
    A_LOADTIMES,
    A_SAVETIMES,
    A_EXPORTTIMES,
    A_CLEARTIMES,
    A_DESELECT_OR_UNDO_BACKOUT,
    A_OVERWRITE_UPDATESCORE,
    A_NAVIGATETONEXTCELL,
    A_COPY,
    A_CUT,
    A_PASTE,
    A_DELETE,
    A_DELETECELLTEXT,
    A_DELETECELLTEXTANDENTERINTO,
    A_DELETESELECTION,
    A_DELETECHAR,
    A_NAVIGATELEFT,
    A_NAVIGATERIGHT,
    A_NAVIGATEUP,
    A_NAVIGATEDOWN,
    A_SWAPVETOCOLOR,

    ACTION_COUNT
} Actions;

#endif