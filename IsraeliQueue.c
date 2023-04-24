#include "IsraeliQueue.h"
#include <stdio.h>

typedef struct FriendshipList {
    FriendshipFunction m_function;
    struct FriendshipList* m_next;
    
} *FriendshipList;

FriendshipList createFriendshipListNode(const FriendshipFunction function)
{
    FriendshipList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_function = function;
    list->m_next = NULL;
    return list;
}

void destroyFrindshipList(FriendshipList listHead)
{
    while(listHead)
    {
        FriendshipList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

typedef struct IsraeliList {
    void* m_item;
    int m_friends;
    int m_rivals;
    struct IsraeliList* m_next;
    
} *IsraeliList;

IsraeliList createIsraeliListNode(const void* item)
{
    IsraeliList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_item = item;
    list->m_friends = 5;
    list->m_rivals = 3;
    list->m_next = NULL;
    return list;
}

void destroyIsraeliList(IsraeliList listHead)
{
    while(listHead)
    {
        IsraeliList toDelete = listHead;
        listHead = listHead->m_next;
        free(toDelete);
    }
}

struct IsraeliQueue_t
{
    int m_rivalry;
    int m_frindshipThershold;
    IsraeliList m_listHead;
    FriendshipList m_listFrindshipHead;
    int m_size;
    ComparisonFunction m_compare;
};

void copyFriendshipFunctions(FriendshipFunction * frindship, FriendshipList listHead)
{
    int counter = 0;
    listHead = createFriendshipListNode(frindship[counter]); //if Null need to check what to do
    FriendshipList runner = listHead;
    while(frindship[counter])
    {
        counter++;
        runner->m_next = createFriendshipListNode(frindship[counter]); // same check
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
    israeliLine->m_listHead = createIsraeliListNode(NULL); //if Null check what to do
    copyFriendshipFunctions(frindship, israeliLine->m_listFrindshipHead);
    israeliLine->m_size = 0;
    israeliLine->m_compare = compare;
    return israeliLine;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item)
{
    if(!item || !q->m_listHead || !q->m_compare)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    IsraeliList runner = q->m_listHead, placeHolder = q->m_listHead;
    bool flag = true;
    while(runner->m_next)
    {
        if(isFriend(runner, item, q))
        {
            placeHolder = runner;
            while(runner->m_next && flag)
            {
                runner = runner->m_next;
                if(isRival(runner, item, q))
                {
                    flag = false;
                }
            }
        }
        if(!flag)
        {
            placeHolder = runner;
            flag = true;
            runner = runner->m_next;
        } 
    }
    if(!flag)
    {
        placeHolder = NULL
    }

    if(insertNode(placeHolder, item))
    {
        return ISRAELIQUEUE_SUCCESS;
    }
    return ISRAELIQUEUE_ALLOC_FAILED; 
}

bool isFriend(const IsraeliList node, const void * item, const IsraeliQueue q)
{
    FriendshipList runner = q->m_listFrindshipHead;
    while(runner)
    {
        if(runner->m_function(item, node->m_item) > q->m_frindshipThershold)
        {
            return true;
        }
        runner = runner->m_next;
    }
    return false;
}

bool isRival(const IsraeliList node, const void * item, const IsraeliQueue q)
{
    FriendshipList runner = q->m_listFrindshipHead;
    int counter = 0, average = 0;
    if(isFriend(node, item, q))
    {
        return false;
    }
    while(runner)
    {
        counter++;
        average += runner->m_function(item,node->m_item);
        runner = runner->m_next;
    }
    return ((double)average/counter) < (double) q->m_rivalry;
}