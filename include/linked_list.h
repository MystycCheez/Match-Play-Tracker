#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

#endif