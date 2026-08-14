#ifndef TEXT_H
#define TEXT_H

#include "structs.h"

void SetText(TextStruct* Text, char* str);
void AppendChar(TextStruct* Text, char c);
bool MoveCursor(TextStruct* Text, int moveDir);
bool SelectChar(TextStruct* Text, int moveDir);
void SelectAll(TextStruct* Text);

#endif