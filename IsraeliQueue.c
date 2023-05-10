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
    FriendshipList list =NULL;
    list = (FriendshipList) malloc(sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->m_function = function;
    list->m_next = NULL;
    return list;
}

void destroyFrindshipList(FriendshipList listHead)
{
    while(listHead != NULL)
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
    if(list == NULL)
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
    while(listHead != NULL)
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
    ComparisonFunction m_compare;
};

IsraeliList findPlaceToEnter (IsraeliQueue q, void * item);
int FindMaxLength(const IsraeliQueue* q);
int FindListSize(const IsraeliQueue* q);
bool MergeFriendship (const IsraeliQueue* q, const IsraeliQueue merged);
int getAverage(IsraeliQueue* q);
int getGeomtricAverage(const IsraeliQueue* q);
IsraeliQueueError IsraeliQueueReenter (IsraeliQueue q, IsraeliList node);


IsraeliQueue IsraeliQueueCreate(FriendshipFunction * frindship, ComparisonFunction compare, int friendshipThershold, 
                                                            int rivalryThershold)
{
    IsraeliQueue line = (IsraeliQueue) malloc(sizeof(*line));
    if(line == NULL || compare == NULL || frindship == NULL)
    {
        return NULL;
    }
    line->m_rivalry=rivalryThershold;
    line->m_frindshipThershold = friendshipThershold;
    line->m_listHead = NULL;
    FriendshipList first =NULL;
    int counter = 0;
    while(frindship[counter] != NULL)
    {
        if(first == NULL)
        {
            line->m_listFrindshipHead = createFriendshipListNode(frindship[counter]);
            if(line->m_listFrindshipHead == NULL)
            {
                IsraeliQueueDestroy(line);
                return NULL;
            }
            first = line->m_listFrindshipHead;
        }
        else
        {
            FriendshipList temp = createFriendshipListNode(frindship[counter]);
            line->m_listFrindshipHead->m_next = temp;
            if(line->m_listFrindshipHead->m_next == NULL)
            {
                IsraeliQueueDestroy(line);
                return NULL;
            }
            line->m_listFrindshipHead = temp;
        }
        counter++;
    }
    line->m_listFrindshipHead = first;
    line->m_compare = compare;
    return line;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item)
{
    if(item == NULL || q == NULL || q->m_compare == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    IsraeliList placeHolder = findPlaceToEnter(q, item);
    
    if(placeHolder == NULL)
    {
        q->m_listHead = createIsraeliListNode(item);
        if(q->m_listHead == NULL)
        {
            return ISRAELIQUEUE_ALLOC_FAILED;
        }
        return ISRAELIQUEUE_SUCCESS;
    }
    IsraeliList next= createIsraeliListNode(item);
    if(next == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    
    IsraeliList temp = placeHolder->m_Right;
    placeHolder->m_Right = next;
    next->m_Right = temp;
    next->m_Left = placeHolder;
    if(next->m_Right != NULL)
    {
        next->m_Right->m_Left = next;
    }
    placeHolder->m_friends++;
    return ISRAELIQUEUE_SUCCESS;
}

bool isFriend(IsraeliList node, void * item, IsraeliQueue q)
{
    if(node->m_friends > NUM_OF_FRIENDS)
    {
        return false;
    }
    FriendshipList runner = q->m_listFrindshipHead;
    while(runner != NULL)
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
    while(runner != NULL)
    {
        counter++;
        average += runner->m_function(item,node->m_item);
        runner = runner->m_next;
    }
    return ((double)average/counter) < (double) q->m_rivalry;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction function)
{
    if(q == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipList runner = q->m_listFrindshipHead;
    while(runner != NULL && runner->m_next != NULL)
    {
        runner = runner->m_next;
    }
    FriendshipList next = createFriendshipListNode(function);
    if(next == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    runner->m_next = next;
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
    if(q == NULL || q->m_listHead == NULL)
    {
        return 0;
    }
    IsraeliList runner = q->m_listHead;
    int counter = 0;
    while(runner != NULL)
    {
        counter++;
        runner = runner->m_Right;
    }
    return counter;
}

bool IsraeliQueueContains(IsraeliQueue q, void * item)
{
    if(item == NULL || q == NULL || q->m_listHead == NULL || q->m_compare == NULL)
    {
        return false;
    }
    IsraeliList runner = q->m_listHead;
    while(runner == NULL)
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
    if(q == NULL || q->m_listHead == NULL || q->m_listHead->m_item == NULL)
    {
        return NULL;
    }
    if(q->m_listHead->m_Right == NULL)
    {
        void * item = q->m_listHead->m_item;
        free(q->m_listHead);
        q->m_listHead =NULL;
        return item;
    }
    IsraeliList head = q->m_listHead;
    q->m_listHead = q->m_listHead->m_Right;
    head->m_Right =NULL;
    head->m_Left =NULL;
    if(q->m_listHead != NULL)
    {
        q->m_listHead->m_Left =NULL;
    }
    void * item = head->m_item;
    free(head);
    return item;
    
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* q, ComparisonFunction compare)
{
    if(q == NULL && compare == NULL)
    {
        return NULL;
    }
    IsraeliQueue merged = IsraeliQueueCreate(NULL, compare,0,0);
    if(merged == NULL)
    {
        return NULL;
    }
    int maxLength = FindMaxLength(q), listSize = FindListSize(q);
    IsraeliList * runners = (IsraeliList *) malloc(sizeof(*runners)*listSize + 1);
    if(!MergeFriendship(q,merged))
    {
        IsraeliQueueDestroy(merged);
        return NULL;
    }
    if(runners == NULL)
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
            if(runners[j] != NULL)
            {
                runners[listSize] = createIsraeliListNode(runners[j]->m_item);
                if(runners[listSize] == NULL)
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
    if(q==NULL || q->m_compare == NULL)
    {
        return NULL;
    }
    IsraeliQueue newQueue = (IsraeliQueue) malloc(sizeof(*newQueue));
    if(newQueue == NULL)
    {
        return NULL;
    }
    if(q->m_listHead != NULL)
    {
        IsraeliList oldListRunner = q->m_listHead;
        IsraeliList firstNode = NULL;
        while(oldListRunner !=NULL)
        {
            if(firstNode == NULL)
            {
                newQueue->m_listHead = createIsraeliListNode(oldListRunner->m_item);
                if(newQueue->m_listHead == NULL)
                {
                    IsraeliQueueDestroy(newQueue);
                    return NULL;
                }
                firstNode =  newQueue->m_listHead;
            }
            else
            {
                newQueue->m_listHead->m_Right = createIsraeliListNode(oldListRunner->m_item);
                if(newQueue->m_listHead->m_Right == NULL)
                {
                    IsraeliQueueDestroy(newQueue);
                    return NULL;
                }
                newQueue->m_listHead = newQueue->m_listHead->m_Right;
            }
            oldListRunner = oldListRunner->m_Right;
        }
        newQueue->m_listHead = firstNode;
    }
    else
    {
        newQueue->m_listHead =NULL;
    }
    if(q->m_listFrindshipHead != NULL)
    {
        FriendshipList oldFriendshipRunner = q->m_listFrindshipHead;
        newQueue->m_listFrindshipHead = createFriendshipListNode(oldFriendshipRunner->m_function);
        if(newQueue->m_listFrindshipHead == NULL)
        {
            IsraeliQueueDestroy(newQueue);
            return NULL;
        }
        FriendshipList firstFriendship =  newQueue->m_listFrindshipHead;
        oldFriendshipRunner = oldFriendshipRunner->m_next;
        while(oldFriendshipRunner != NULL && newQueue->m_listFrindshipHead != NULL)
        {
            newQueue->m_listFrindshipHead->m_next = createFriendshipListNode(oldFriendshipRunner->m_function);
            if(newQueue->m_listFrindshipHead->m_next == NULL)
            {
                IsraeliQueueDestroy(newQueue);
                return NULL;
            }
            newQueue->m_listFrindshipHead = newQueue->m_listFrindshipHead->m_next;
            oldFriendshipRunner = oldFriendshipRunner->m_next;
        }
        newQueue->m_listFrindshipHead = firstFriendship;
    }
    else
    {
        newQueue->m_listFrindshipHead = NULL;
    }
    newQueue->m_compare = q->m_compare;
    IsraeliQueueUpdateFriendshipThreshold(newQueue, q->m_frindshipThershold);
    IsraeliQueueUpdateRivalryThreshold(newQueue, q->m_rivalry);
    return newQueue;
}

bool MergeFriendship (const IsraeliQueue* q, const IsraeliQueue merged)
{
    int counterList = 0;
    FriendshipList runnerMerged = merged->m_listFrindshipHead;
        while(q[counterList] != NULL)
        {
            FriendshipList runnerList = q[counterList]->m_listFrindshipHead;
            while (runnerList != NULL)
            {
                    runnerMerged = createFriendshipListNode(runnerList->m_function);
                    if(runnerMerged == NULL)
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
    while(q[counter] != NULL)
    {
        average += q[counter]->m_frindshipThershold;
        counter++;
    }
    return (int) ceil((double) average/ (double) counter);
}

int getGeomtricAverage(const IsraeliQueue* q)
{
    int average = 1, counter = 0;
    while(q[counter] != NULL)
    {
        average *= abs(q[counter]->m_rivalry);
        counter++;
    }
    return (int) ceil(exp(log(average)/counter));

}

int FindMaxLength(const IsraeliQueue* q)
{
    int max = 0, counter = 0;
    while(q[counter] != NULL)
    {
        int size = IsraeliQueueSize(q[counter]);
        if(size > max)
        {
            max = size;
        }
        counter++;
    }
    return max;
}

int FindListSize(const IsraeliQueue* q)
{
    int counter = 0;
    while(q[counter] != NULL)
    {
        counter++;
    }
    return counter;
}

void IsraeliQueueDestroy(IsraeliQueue q)
{
    if(q == NULL)
    {
        return;
    }
    destroyIsraeliList(q->m_listHead);
    destroyFrindshipList(q->m_listFrindshipHead);
    free(q);
}


IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q)
{
    if(q == NULL || q->m_listHead == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    IsraeliList runner = q->m_listHead;
    while(runner != NULL)
    {
        runner = runner->m_Right;
    }
    int size = IsraeliQueueSize(q);
    for (int i = size; i >= 0; i--)
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
    if(item == NULL || q->m_listHead == NULL || q->m_compare == NULL || q == NULL)
    {
        return NULL;
    }
    IsraeliList runner = q->m_listHead, placeHolder = q->m_listHead;
    bool foundRival = false;
    bool found = false;
    while(!found && runner->m_Right != NULL)
    {
        if(isFriend(runner, item, q))
        {
            placeHolder = runner;
            while(runner->m_Right != NULL && !foundRival)
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
                placeHolder = runner;
            }
            else
            {
                found = true;
            }
        }
        if(!found && runner->m_Right != NULL)
        {   
            runner = runner->m_Right;
        }
        else
        {
            placeHolder = runner;
        }
    }
    if(isRival(placeHolder, item, q))
    {
        return runner;
    }
    return placeHolder;
}

IsraeliQueueError IsraeliQueueReenter (IsraeliQueue q, IsraeliList node)
{
    IsraeliList placeHolder = findPlaceToEnter (q, node->m_item);
    if(placeHolder == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    node->m_Left = placeHolder;
    node->m_Right = placeHolder->m_Right;
    placeHolder->m_Right = node;
    node->m_Right->m_Left = node;
    return ISRAELIQUEUE_SUCCESS;
}