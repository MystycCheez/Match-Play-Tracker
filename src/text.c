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

void CopyText(char* text)
{
    SDL_SetClipboardText(text);
}

#endif