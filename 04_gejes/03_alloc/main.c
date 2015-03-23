#include <stdio.h>
#include <string.h>

#define PAGESIZE 1024

struct memPart
{
    int size;
    struct memPart* next;
    struct memPart* prev;
    int free;
};
void* myMalloc(int size)
{
    static struct memPart* root;
    if (root == NULL)
    {
        root = (struct memPart*)sbrk(PAGESIZE);
        root->size = PAGESIZE - sizeof(struct memPart);
        root->free = 0; //free part
        root->prev = NULL;
        root->next = NULL;
    }

    struct memPart* nextPart = root; //part, which we will return
    while (nextPart->next != NULL)
    {
        nextPart = nextPart->next;
    }

    struct memPart* lessPart = root; //part, which we will split or
    while (lessPart != NULL && (lessPart->free != 0 || lessPart->size < size + sizeof(struct memPart)))
        lessPart = lessPart->next;

    if (lessPart == NULL)
    {
        printf("myMalloc: Out of memory. Getting bigger...\n");
        nextPart->next = (struct memPart*)sbrk(PAGESIZE);
        nextPart->next->free = 0;
        nextPart->next->size = PAGESIZE - sizeof(struct memPart);
        nextPart->next->prev = nextPart;
        nextPart->next->next = NULL;
        nextPart = nextPart->next;

        printf("Ok.\n");
        lessPart = root;    //try again
        while (lessPart != NULL && (lessPart->free != 0 || lessPart->size < size + sizeof(struct memPart)))
            lessPart = lessPart->next;
    }
    if (lessPart == NULL)
    {
        fprintf(stderr, "myMalloc: Out of memory. Call is too big.\nFatal error\n");
        return NULL;
    }

    nextPart->next = (struct memPart*)((void*)lessPart + lessPart->size - size);
    lessPart->size = lessPart->size - size - sizeof(struct memPart);

    nextPart->next->next = NULL;
    nextPart->next->prev = nextPart;
    nextPart->next->size = size;
    nextPart->next->free = -1;

    struct memPart* nowPart = root;
    /*while(nowPart != NULL)
    {
        printf("%d\t%d\t (%d): %d\n", (int)nowPart, (int)((int)nowPart + sizeof(struct memPart) + nowPart->size),
            nowPart->size, nowPart->free);
        nowPart = nowPart->next;
    }*/

    return (void*)(nextPart->next) + sizeof(struct memPart);
}

int getNeisMem(struct memPart* ptr, struct memPart** realPrev, struct memPart** realNext) //search for real neighboors in memory
{
    struct memPart* root = ptr;
    while (root->prev != NULL)
    {
        root = (struct memPart*)(root->prev);
    }
    *realPrev = root;
    *realNext = root;

    while (*realPrev != NULL && (void*)(*realPrev) + ((*realPrev)->size) + sizeof(struct memPart) != ptr)
        *realPrev = (*realPrev)->next;
    while (*realNext != NULL && (void*)(*realNext) != (void*)ptr + sizeof(struct memPart) + (ptr->size))
        *realNext = (*realNext)->next;

    return 0;
}

int myFree(void* ptr)
{
    struct memPart* delPart = (struct memPart*)(ptr - sizeof(struct memPart));
    if (delPart == NULL)
    {
        fprintf(stderr, "myFree: Error - null pointer.\n");
        return -1;
    }

    struct memPart* delPrev = NULL;
    struct memPart* delNext = NULL;
    getNeisMem(delPart, &delPrev, &delNext);
    delPart->free = 0;

    if (delPrev != NULL && delPrev->free == 0) //merge with previous
    {
        delPrev->size = delPrev->size + (delPart->size) + sizeof(struct memPart);
        //printf("merge left\n");
        if (delPart->prev != NULL)
            delPart->prev->next = delPart->next;
        if (delPart->next != NULL)
            delPart->next->prev = delPart->prev;
        delPart = delPrev;
    }
    if (delNext != NULL && delNext->free == 0) //merge with next
    {
        delPart->size = delPart->size + (delNext->size) + sizeof(struct memPart);
        //printf("merge right\n");
        if (delNext->prev != NULL)
            delNext->prev->next = delNext->next;
        if (delNext->next != NULL)
            delNext->next->prev = delNext->prev;
    }
    return 0;
}

void getStat(void* ptr)
{
        int freeBytes = 0;
        int useBytes = 0;
        int allBytes = 0;
        int partCount = 0;
        struct memPart* nowPart = (struct memPart*)((void*)ptr - sizeof(struct memPart));
        while (nowPart->prev != NULL)
                nowPart = nowPart->prev;
        while (nowPart != NULL)
        {
                if (nowPart->free == 0)
                    printf("+free: ");
                else
                    printf("-used: ");
                printf("%d bytes\t(%d w/o struct)\n", nowPart->size + sizeof(struct memPart), nowPart->size);
                if (nowPart->free == 0)
                    freeBytes = freeBytes + nowPart->size;
                else
                    useBytes = useBytes + nowPart->size;
                useBytes = useBytes + sizeof(struct memPart);
                allBytes = allBytes + nowPart->size + sizeof(struct memPart);
                partCount++;
                nowPart = nowPart->next;
        }
        printf("%d parts:\n%d free\t%d use\nall: %d bytes.\n", partCount, freeBytes, useBytes, allBytes);
}



int main(int argc, char** argv)
{
    char* com = (char*)myMalloc(10*sizeof(char));
    int i = 0;
    for (i = 0; i < 10; ++i)
        com[i] = 0;
    int n = 0;
    scanf("%d", &n);
    char** str = (char**)myMalloc(n*sizeof(char*));
    for (i = 0; i < n; ++i)
    {
        int j = 0;
        printf("%d: ", i);
        char* newStr = (char*)myMalloc(255*sizeof(char));
        scanf("%s", newStr);
        str[i] = (char*)myMalloc(strlen(newStr)*sizeof(char));
        for (j = 0; j < strlen(newStr); ++j)
            str[i][j] = newStr[j];
        str[i][strlen(newStr) - 1] = 0;
        myFree(newStr);
        //printf("Memory OK.\n");
    }
    while (strcmp(com, "end") != 0)
    {
        printf("new command: ");
        scanf("%s", com);
        if (strcmp(com, "free") == 0)
        {
            scanf("%d", &i);
            myFree(str[i]);
            printf("Memory freed.\n");
        }
        if (strcmp(com, "stat") == 0) //only for statistic
        {
            getStat(str);
        }
    }

    //printf("%s\n", str);

    //myFree(str);
    return 0;
}
