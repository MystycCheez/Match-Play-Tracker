#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>

extern void initLinkedList(void* data, const char *file, int line, const char *func);
extern void* debug_malloc(size_t size, const char *file, int line, const char *func);
extern void debug_free(void* p);
extern void checkUnfreed();

#endif