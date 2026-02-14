#ifndef HANDLER_H
#define HANDLER_H

#include "structs.h"

void ExitHandler();
void MouseTitleBarHandler(CollisionMap Collision, Vector2 windowPos);
void MouseSheetHandler();
void MouseHandler();
void updateCollider();
void updateCollisionMap();
void MouseHandler();
void EnterNavigationHandler();
void CellOverwriteHandler();
void CellInputHandler();
void KeyHandler();
void CursorHandler();

#endif