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
    if(!function)
    {
        printf("null function\n");
    }
    FriendshipList list = malloc (sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    list->m_function = function;
    list->m_next = NULL;
    printf("succeded in making a node\n");
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
    if(!frindship || !frindship[0])
    {
        return true;
    }
    int sizeOfTotalBytes = sizeof(frindship), sizeOfFirstPointer = sizeof(frindship[0]);
    int sizeOfArray = sizeOfTotalBytes/sizeOfFirstPointer;
    listHead = createFriendshipListNode(frindship[0]); 
    if(!listHead)
    {
        return false;
    }
    FriendshipList runner = listHead;
    for(int i = 0; i < sizeOfArray; i++)
    {
        runner->m_next = createFriendshipListNode(frindship[i]); 
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
    if(!item)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
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
    if(!q)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipList runner = q->m_listFrindshipHead;
    while(runner->m_next)
    {
        runner = runner->m_next;
    }
    FriendshipList next = createFriendshipListNode(function);
    if(!next)
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
    printf("given queue not null \n");
    IsraeliQueue newQueue = (IsraeliQueue) malloc(sizeof(newQueue));
    if(!newQueue)
    {
        return NULL;
    }
    printf("malloc succeded\n");
    IsraeliList oldListRunner = q->m_listHead;
    IsraeliList firstNode = NULL;
    while(oldListRunner)
    {
        if(!firstNode)
        {
            newQueue->m_listHead = createIsraeliListNode(oldListRunner->m_item);
            if(!newQueue->m_listHead)
            {
                 IsraeliQueueDestroy(newQueue);
                return NULL;
            }
            firstNode =  newQueue->m_listHead;
        }
        else
        {
            newQueue->m_listHead->m_Right = createIsraeliListNode(oldListRunner->m_item);
            if(!newQueue->m_listHead->m_Right)
            {
                 IsraeliQueueDestroy(newQueue);
                return NULL;
            }
            newQueue->m_listHead = newQueue->m_listHead->m_Right;
        }
        oldListRunner = oldListRunner->m_Right;
    }
    newQueue->m_listHead = firstNode;
    printf("copied nodes\n");
    FriendshipList oldFriendshipRunner = q->m_listFrindshipHead;
    FriendshipList firstFriendship = NULL;
    if(oldFriendshipRunner)
    {
        printf("old friendship list not null\n");
        while(oldFriendshipRunner)
        {
            printf("started loop run\n");
            if(!firstFriendship)
            {
                
                if(!oldFriendshipRunner->m_function)
                {
                    printf("function null\n");
                }
                printf("trying to create new friendship list\n");
                newQueue->m_listFrindshipHead = createFriendshipListNode(oldFriendshipRunner->m_function);
                if(!newQueue->m_listFrindshipHead)
                {
                    IsraeliQueueDestroy(newQueue);
                    return NULL;
                }
                firstFriendship =  newQueue->m_listFrindshipHead;
                printf("created first friendship list\n");
            }
            else
            {
                printf("trying to create new friendship list\n");
                newQueue->m_listFrindshipHead->m_next = createFriendshipListNode(oldFriendshipRunner->m_function);
                if(!newQueue->m_listFrindshipHead->m_next)
                {
                    IsraeliQueueDestroy(newQueue);
                    return NULL;
                }
                newQueue->m_listFrindshipHead = newQueue->m_listFrindshipHead->m_next;
                printf("created first friendship list\n");
            }
            oldFriendshipRunner = oldFriendshipRunner->m_next;
            if(oldFriendshipRunner)
            {
                printf("there is a node\n");
            }
        }
        newQueue->m_listFrindshipHead = firstFriendship;
        printf("finished loop run\n");
    }
    printf("copied friendship lists\n");
    IsraeliQueueUpdateFriendshipThreshold(newQueue, q->m_frindshipThershold);
    IsraeliQueueUpdateRivalryThreshold(newQueue, q->m_rivalry);
    newQueue->m_size = IsraeliQueueSize(q);
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