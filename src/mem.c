#ifndef MEM_C
#define MEM_C

#include "headers.h"

char* strCreate(size_t len)
{
    char* str = malloc(len);
    memset(str, 0, len);
    return str;
}

#endif