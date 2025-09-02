#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"

extern Node* MNode;

// Create new Node for a doubly linked list
Node* NewNode(void* data, Node* next, Node* prev, const char *file, int line, const char *func)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("Error! Could not allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = next;
    node->prev = prev;
    node->file = file;
    node->line = line;
    node->func = func;

    return node;
}

void initLinkedList(void* data, const char *file, int line, const char *func)
{
    MNode = NewNode(data, NULL, NULL, NULL, 0, NULL);
    MNode->next = MNode;
    MNode->prev = MNode;
    MNode->file = file;
    MNode->line = line;
    MNode->func = func;
    printf("Created new list\n");
}

void* debug_malloc(size_t size, const char *file, int line, const char *func)
{
    #undef malloc
    void *p = malloc(size);
    #define malloc(X) debug_malloc(X, __FILE__, __LINE__, __FUNCTION__)
    // fprintf(log, "Allocating: %s, %d, %s, %p[%lld]\n", file, line, func, p, size);
    (void)file;
    (void)line;
    (void)func;
    
    if (MNode == NULL) {
        initLinkedList(p, file, line, func);
    } else if (MNode->next == MNode) {
        MNode->next = NewNode(p, MNode, MNode, file, line, func);
    } else {
        MNode->next = NewNode(p, MNode->next, MNode, file, line, func);
    }
    MNode = MNode->next;
    return p;
}

void debug_free(void* p, const char *file, int line, const char *func)
{
    // printf("Mnode->data: %s\n", (char*)MNode->data);
    // printf("p:           %s\n", (char*)p);
    // printf("De-allocating: %p, %s, %d, %s\n", p, file, line, func);
    (void)file;
    (void)line;
    (void)func;

    Node* current = MNode;
    while (current->data != p) {
        current = current->next;
    }
    if (current->next != current) {
        current->next->prev = current->prev;
        current->prev->next = current->next;
        MNode = current->next;
    }

    #undef free
    free(current);
    free(p);
    #define free(X) debug_free(X, __FILE__, __LINE__, __FUNCTION__)
}

void checkUnfreed()
{
    char* filename = "malloc_log.txt";
    FILE* log = fopen(filename, "w");
    if (log == NULL) {
        fprintf(stderr, "Could not open %s\n", filename);
    }
    Node* current = MNode;
    size_t nodeCount = 0;
    #undef free
    while (current->next != MNode) {
        current = current->next;
        free(current->prev);
        nodeCount++;
        fprintf(log, "%s:%d, %s\n", current->file, current->line, current->func);
    }
    if (current->next == MNode) {
        free(current);
        nodeCount++;
        fprintf(log, "%s:%d, %s\n", current->file, current->line, current->func);
    }

    printf("%lld nodes remain unfreed.\n", nodeCount);
    #define free(X) debug_free(X, __FILE__, __LINE__, __FUNCTION__)
    fclose(log);
}