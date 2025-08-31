#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"

bool TraverseNodeForward(Node** currentNode)
{
    assert(*currentNode != NULL);
    if ((*currentNode)->next != NULL) {*currentNode = (*currentNode)->next; return true;}
    else return false;
}

bool TraverseNodeBackward(Node** currentNode)
{
    assert(*currentNode != NULL);
    if ((*currentNode)->prev != NULL) {*currentNode = (*currentNode)->prev; return true;}
    else return false;
}

// Create new Node for a doubly linked list with NULL prev and next
Node* NewNode(void* data)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("Error! Could not allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

// Pass what should be the tail to this function to remove it
void RemoveTailNode(Node* tail)
{
    if(tail == NULL)        {exit(EXIT_FAILURE);} // Don't pass a null node lol
    if(tail->prev != NULL)  {exit(EXIT_FAILURE);} // If this is the case, it's not the tail!
    free(tail);
}

// Traverses from current node to head to remove the end and make the head->prev the new head
void RemoveHeadNode(Node* currentNode)
{
    assert(currentNode != NULL); // If this happens, Something went wrong
    assert(currentNode->next != NULL); // Don't call this function if current is also the end!
    while (currentNode->next != NULL) {TraverseNodeForward(&currentNode);}
    if (currentNode->prev != NULL) {currentNode->prev->next = NULL;}
    free(currentNode);
}

// Removes all next nodes after current
void ClearNextNodes(Node* currentNode)
{
    assert(currentNode != NULL);
    while (currentNode->next != NULL) {RemoveHeadNode(currentNode);}
}

// This replaces the currentNode's next node to a new node
void ReplaceNextNode(Node** currentNode, void* data)
{
    Node* newNode = NewNode(data);
    (*currentNode)->next = newNode;
    newNode->prev = *currentNode;
}