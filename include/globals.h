#ifndef GLOBALS_H
#define GLOBALS_H

#include "structs.h"
#include "defines.h"
#include "enums.h"

extern Globals GVARS;
extern UI_Elements UI;
extern Window_Elements Window;
extern TitleBar_Elements TitleBar;
extern Cursor_Elements Cursor;
extern MouseState Mouse;
extern KeyboardState KeyData;
extern Cell* sheet;

extern Actions ActionTable[3][KEY_COMBO_COUNT];

// List of action functions
extern void (*Action[ACTION_COUNT])();

#endif