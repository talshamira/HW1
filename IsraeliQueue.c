#include "IsraeliQueue.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define NUM_OF_FRIENDS 5
#define NUM_OF_RIVALS 3
 



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
    bool m_moved;
    struct IsraeliList* m_Right;
    struct IsraeliList* m_Left;
    
} *IsraeliList;

IsraeliList createIsraeliListNode(void* item)
{
    IsraeliList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_item = item;
    list->m_friends = NUM_OF_FRIENDS;
    list->m_rivals = NUM_OF_RIVALS;
    list->m_moved = false;
    list->m_Right = NULL;
    list->m_Left = NULL;
    return list;
}

void destroyIsraeliList(IsraeliList listHead)
{
    while(listHead)
    {
        IsraeliList toDelete = listHead;
        listHead = listHead->m_Right;
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

IsraeliList findPlaceToEnter (IsraeliQueue q, void * item);
int FindMaxLength(const IsraeliQueue* q);
IsraeliQueueError insertNode (IsraeliList node,IsraeliQueue q, void * item);
int FindListSize(const IsraeliQueue* q);
bool MergeFriendship (const IsraeliQueue* q, const IsraeliQueue merged);
int getAverage(IsraeliQueue* q);
int getGeomtricAverage(const IsraeliQueue* q);
IsraeliQueueError IsraeliQueueReenter (IsraeliQueue q, IsraeliList node);

bool copyFriendshipFunctions(FriendshipFunction * frindship, FriendshipList listHead)
{
    int counter = 0;
    listHead = createFriendshipListNode(frindship[counter]); 
    if(!listHead)
    {
        return false;
    }
    FriendshipList runner = listHead;
    while(frindship[counter])
    {
        counter++;
        runner->m_next = createFriendshipListNode(frindship[counter]); 
        if(!runner->m_next)
        {
            return false;
        }
        runner = runner->m_next;
    }
    return true;
}

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * frindship, ComparisonFunction compare, 
                                                                int friendshipThershold, int rivalryThershold)
{
    IsraeliQueue israeliLine = malloc (sizeof(israeliLine));
    if(!israeliLine)
    {
        return NULL;
    }
    israeliLine->m_rivalry=rivalryThershold;
    israeliLine->m_frindshipThershold = friendshipThershold;
    israeliLine->m_listHead = createIsraeliListNode(NULL); 
    if(!israeliLine->m_listHead)
    {
        IsraeliQueueDestroy(israeliLine);
        return NULL;
    }
    if(!copyFriendshipFunctions(frindship, israeliLine->m_listFrindshipHead))
    {
        IsraeliQueueDestroy(israeliLine);
        return NULL; 
    }
    israeliLine->m_size = 0;
    israeliLine->m_compare = compare;
    return israeliLine;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item)
{
    IsraeliList placeHolder = findPlaceToEnter(q, item);
    if(!placeHolder)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    return insertNode(placeHolder, q, item);    
}

IsraeliQueueError insertNode (IsraeliList node,IsraeliQueue q, void * item)
{
    IsraeliList next= createIsraeliListNode(item);
    if(!next)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    IsraeliList temp = node->m_Right;
    node->m_Right = next;
    next->m_Right = temp;
    node->m_friends++;
    q->m_size++;
    return ISRAELIQUEUE_SUCCESS;
}

bool isFriend(IsraeliList node, void * item, IsraeliQueue q)
{
    if(node->m_friends > NUM_OF_FRIENDS)
    {
        return false;
    }
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

bool isRival(IsraeliList node, void * item,  IsraeliQueue q)
{
    if(node->m_rivals > NUM_OF_RIVALS)
    {
        return false;
    }
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

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction function)
{
    FriendshipList next = createFriendshipListNode(function);
    if(!next)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    next->m_next = q->m_listFrindshipHead;
    q->m_listFrindshipHead = next;
    return ISRAELIQUEUE_SUCCESS;
}


IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int thershold)
{
    q->m_frindshipThershold = thershold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int thershold)
{
    q->m_rivalry = thershold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue q)
{
    return q->m_size;
}

bool IsraeliQueueContains(IsraeliQueue q, void * item)
{
    if(!item || !q || !q->m_listHead || !q->m_compare)
    {
        return false;
    }
    IsraeliList runner = q->m_listHead;
    while(runner)
    {
        if(q->m_compare(runner,item))
        {
            return true;
        }
        runner = runner->m_Right;
    }
    return false;
}

void* IsraeliQueueDequeue(IsraeliQueue q)
{
    if(!q || !q->m_listHead)
    {
        return NULL;
    }
    IsraeliList head = q->m_listHead;
    q->m_listHead = q->m_listHead->m_Right;
    q->m_size--;
    return head;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* q, ComparisonFunction compare)
{
    if(!q && !compare)
    {
        return NULL;
    }
    IsraeliQueue merged = IsraeliQueueCreate(NULL, compare,0,0);
    if(!merged)
    {
        return NULL;
    }
    int maxLength = FindMaxLength(q), listSize = FindListSize(q);
    IsraeliList * runners = (IsraeliList *) malloc(sizeof(runners)*listSize + 1);
    if(!MergeFriendship(q,merged))
    {
        IsraeliQueueDestroy(merged);
        return NULL;
    }
    if(!runners)
    {
        IsraeliQueueDestroy(merged);
        return NULL;
    }
    for (int i = 0; i < listSize; i++)
    {
        runners[i] = q[i]->m_listHead;
    }
    runners[listSize] = merged->m_listHead;
    for(int i = 0; i < maxLength; i++)
    {
        for(int j = 0; j < listSize; j++)
        {
            if(runners[j])
            {
                runners[listSize] = createIsraeliListNode(runners[j]->m_item);
                if(!runners[listSize])
                {
                    IsraeliQueueDestroy(merged);
                    return NULL;
                }
                runners[listSize] = runners[listSize]->m_Right;
            }
            runners[j] = runners[j]->m_Right;
        }
    }
    merged->m_frindshipThershold = getAverage(q);
    merged->m_rivalry = getGeomtricAverage(q);
    return merged;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    if(!q)
    {
        return NULL;
    }
    IsraeliQueue newQueue = IsraeliQueueCreate(NULL, q->m_compare, q->m_frindshipThershold, q->m_rivalry);
    if(!newQueue)
    {
        return NULL;
    }
    IsraeliList oldListRunner = q->m_listHead;
    IsraeliList newListRunner = newQueue->m_listHead;
    while(oldListRunner)
    {
        newListRunner = createIsraeliListNode(oldListRunner->m_item);
        if(!newListRunner)
        {
            IsraeliQueueDestroy(newQueue);
            return NULL;
        }
        oldListRunner = oldListRunner->m_Right;
        newListRunner = newListRunner->m_Right;
    }
    FriendshipList oldFriendshipRunner = q->m_listFrindshipHead;
    FriendshipList newfriendshipRunner = newQueue->m_listFrindshipHead;
    while(oldFriendshipRunner)
    {
        newfriendshipRunner = createFriendshipListNode(oldFriendshipRunner->m_function);
        if(!newfriendshipRunner)
        {
           IsraeliQueueDestroy(newQueue);
            return NULL; 
        }
        oldFriendshipRunner = oldFriendshipRunner->m_next;
        newfriendshipRunner = newfriendshipRunner->m_next;
    }
    return newQueue;
}

bool MergeFriendship (const IsraeliQueue* q, const IsraeliQueue merged)
{
    int counterList = 0;
    FriendshipList runnerMerged = merged->m_listFrindshipHead;
        while(q[counterList])
        {
            FriendshipList runnerList = q[counterList]->m_listFrindshipHead;
            while (runnerList)
            {
                    runnerMerged = createFriendshipListNode(runnerList->m_function);
                    if(!runnerMerged)
                    {
                        return false;
                    }
                    runnerMerged = runnerMerged->m_next;
                    runnerList = runnerList->m_next;
            }
            counterList++;
        }
    return true;
}

int getAverage(IsraeliQueue* q)
{
    int counter = 0, average = 0;
    while(q[counter])
    {
        average += q[counter]->m_frindshipThershold;
        counter++;
    }
    return (int) ceil((double) average/ (double) counter);
}

int getGeomtricAverage(const IsraeliQueue* q)
{
    int average = 1, counter = 0;
    while(q[counter])
    {
        average *= abs(q[counter]->m_rivalry);
        counter++;
    }
    return (int) ceil(exp(log(average)/counter));

}

int FindMaxLength(const IsraeliQueue* q)
{
    int max = 0, counter = 0;
    while(q[counter])
    {
        if(q[counter]->m_size > max)
        {
            max = q[counter]->m_size;
        }
        counter++;
    }
    return max;
}

int FindListSize(const IsraeliQueue* q)
{
    int counter = 0;
    while(q[counter])
    {
        counter++;
    }
    return counter;
}

void IsraeliQueueDestroy(IsraeliQueue q)
{
    if(!q)
    {
        return;
    }
    if(q->m_listHead)
    {
        destroyIsraeliList(q->m_listHead);
    }
    if(q->m_listFrindshipHead)
    {
        destroyFrindshipList(q->m_listFrindshipHead);
    }
    free(q);
}


IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q)
{
    if(!q || !q->m_listHead)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    IsraeliList runner = q->m_listHead;
    while(runner)
    {
        runner = runner->m_Right;
    }
    
    for (int i = q->m_size-1; i >= 0; i--)
    {
        while(runner->m_moved)
        {
            runner = runner->m_Left;
        }
        IsraeliList tmp = runner;
        runner = runner->m_Left;
        runner->m_Right = tmp->m_Right;
        IsraeliQueueError error = IsraeliQueueReenter(q,tmp);
        if(error != ISRAELIQUEUE_SUCCESS)
        {
            return error;
        }
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliList findPlaceToEnter (IsraeliQueue q, void * item)
{
    if(!item || !q->m_listHead || !q->m_compare)
    {
        return NULL;
    }
    IsraeliList runner = q->m_listHead, placeHolder = q->m_listHead;
    bool foundRival = false;
    bool found = false;
    while(!found && runner->m_Right)
    {
        if(isFriend(runner, item, q))
        {
            placeHolder = runner;
            while(runner->m_Right && !foundRival)
            {
                runner = runner->m_Right;
                if(isRival(runner, item, q))
                {
                    foundRival = true;
                }
            }
            if(foundRival)
            {
                foundRival = false;
            }
            else
            {
                found = true;
            }
        }
        if(!found)
        {   
            runner = runner->m_Right;
        }
        else
        {
            placeHolder = runner;
        }
    }
    return placeHolder;
}

IsraeliQueueError IsraeliQueueReenter (IsraeliQueue q, IsraeliList node)
{
    IsraeliList placeHolder = findPlaceToEnter (q, node->m_item);
    if(!placeHolder)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    node->m_Left = placeHolder;
    node->m_Right = placeHolder->m_Right;
    placeHolder->m_Right = node;
    node->m_Right->m_Left = node;
    return ISRAELIQUEUE_SUCCESS;
}