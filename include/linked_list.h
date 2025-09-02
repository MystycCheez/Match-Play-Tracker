#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    void* data;
    const char *file;
    int line;
    const char *func;
    struct Node* next;
    struct Node* prev;
} Node;

#endif