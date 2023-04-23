#include "IsraeliQueue.h"
#include <stdio.h>

typedef struct node {
    void *m_data;
    struct node* m_next;
    
} *Node;

Node createNode(void * data)
{
    Node ptr = malloc (sizeof(*ptr));
    if(!ptr)
    {
        return NULL;
    }
    ptr->m_data = data;
    ptr->m_next = NULL;
    return ptr;
}

void destroyNode(Node nodeHead)
{
    while(nodeHead)
    {
        Node toDelete = nodeHead;
        nodeHead = nodeHead->m_next;
        free(toDelete);
    }
}

struct IsraeliQueue_t
{
    int m_rivalry;
    int m_frindshipThershold;
    Node m_listHead;
    Node m_listFrindshipHead;
    int m_size;
    ComparisonFunction m_compare;
};

void copyFriendshipFunctions(const FriendshipFunction * frindship, Node listHead)
{
    int counter = 0;
    listHead = createNode(frindship[counter]); //if Null need to check what to do
    Node runner = listHead;
    while(frindship[counter])
    {
        counter++;
        runner->m_next = createNode(frindship[counter]);
        runner = runner->m_next;
    }
}

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * frindship, ComparisonFunction compare, 
                                                                int friendshipThershold, int rivalryThershold)
{
    IsraeliQueue israeliLine = malloc (sizeof(israeliLine));
    if(!&israeliLine)
    {
        return NULL;
    }
    israeliLine->m_rivalry=rivalryThershold;
    israeliLine->m_frindshipThershold = friendshipThershold;
    israeliLine->m_listHead = createNode(NULL); //if Null check what to do
    copyFriendshipFunctions(frindship, israeliLine->m_listFrindshipHead);
    israeliLine->m_size = 0;
    israeliLine->m_compare = compare;
    return israeliLine;
}

