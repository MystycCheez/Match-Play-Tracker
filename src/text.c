#ifndef TEXT_C
#define TEXT_C

#include <string.h>

#include <SDL3/SDL_clipboard.h>

#include "structs.h"

void SetText(TextStruct* Text, char* str)
{
    strcpy(Text->str, str);
}

void AppendChar(TextStruct* Text, char c)
{
    for (size_t i = 0; i < CELL_TEXT_LENGTH; i++) {
        if (!Text->str[i]) {
            Text->str[i] = c;
            return;
        }
    }
}

// Returns true if movement occured
bool MoveCursor(TextStruct* Text, int moveDir)
{
    bool conditions = 
    (Text->cursor + moveDir >= 0) ||
    (Text->cursor + moveDir < Text->len);
    if (conditions) {
        Text->cursor += moveDir;
        Text->anchor += moveDir;
        return true;
    }
    return false;
}

// Returns true if movement occured
bool SelectChar(TextStruct* Text, int moveDir)
{
    bool conditions = 
    (Text->cursor + moveDir >= 0) ||
    (Text->cursor + moveDir < Text->len);
    if (conditions) {
        Text->cursor += moveDir;
        return true;
    }
    return false;
}

void SelectAll(TextStruct* Text)
{
    Text->cursor = 0;
    Text->anchor = 0;

    while (Text->cursor > Text->len) {
        Text->cursor++;
    }
}

void CopyText(char* text)
{
    SDL_SetClipboardText(text);
}

#endif