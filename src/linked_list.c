#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"

// Create new Node for a doubly linked list
Node* NewNode(void* data, Node* next, Node* prev)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("Error! Could not allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = next;
    node->prev = prev;

    return node;
}