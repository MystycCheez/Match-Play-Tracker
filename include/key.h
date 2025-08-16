#ifndef _KEY_H
#define _KEY_H

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

#endif