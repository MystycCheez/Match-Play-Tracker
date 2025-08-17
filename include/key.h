#ifndef _KEY_H
#define _KEY_H

#define KEY_LIST \
    KEY(K_LEFT, KEY_LEFT, "LEFT") \
    KEY(K_RIGHT, KEY_RIGHT, "RIGHT") \
    KEY(K_UP, KEY_UP, "UP") \
    KEY(K_DOWN, KEY_DOWN, "DOWN") \
    KEY(K_ENTER, KEY_ENTER, "ENTER") \
    KEY(K_ESCAPE, KEY_ESCAPE, "ESCAPE") \
    KEY(K_DELETE, KEY_DELETE, "DELETE") \
    KEY(K_BACKSPACE, KEY_BACKSPACE, "BACKSPACE") \
    KEY(K_TAB, KEY_TAB, "TAB") \
    KEY(K_HOME, KEY_HOME, "HOME") \
    KEY(K_END, KEY_END, "END") \
    KEY(K_A, KEY_A, "A") \
    KEY(K_C, KEY_C, "C") \
    KEY(K_D, KEY_D, "D") \
    KEY(K_X, KEY_X, "X") \
    KEY(K_V, KEY_V, "V") \
    KEY(K_L, KEY_L, "L") \
    KEY(K_S, KEY_S, "S") \
    KEY(K_B, KEY_B, "B") \

typedef enum Key {
    #define KEY(a,b,c) a,
        KEY_LIST
    #undef KEY
    KEY_COUNT
} Key;

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

#endif