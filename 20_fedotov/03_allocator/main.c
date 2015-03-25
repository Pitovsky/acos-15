//
//  main.c
//  Allocator
//
//  Created by Simon Fedotov on 23.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct memNode {
    int Size;
    int Occupied;
    struct memNode* next;
    struct memNode* prev;
};


void* OurMalloc(int size) {
    static struct memNode* Root;
    if(Root == NULL) {
        Root = (struct memNode*) sbrk(1024 * 1024);
        Root -> Size = 1024 * 1024 - sizeof(struct memNode*);
        Root -> prev = NULL;
        Root -> next = NULL;
        Root -> Occupied = 0;
    }
    
    
    struct memNode* FreeNode = Root;
    while (FreeNode != NULL && (FreeNode -> Size < size + sizeof(struct memNode) || FreeNode -> Occupied == 1) ) {
        FreeNode = FreeNode -> next;
    }
    
    if(FreeNode == NULL) {
        printf("There is no part with this memory size. Now we're calling sbrk\n");
        struct memNode* lastNode = Root;
        while (lastNode -> next != NULL) {
            lastNode = lastNode -> next;
        }
        lastNode -> next = (struct memNode*) sbrk(1.5 * size);
        lastNode -> next -> Size = 1.5 * size - sizeof(struct memNode*);
        lastNode -> next -> prev = lastNode;
        lastNode -> next -> next = NULL;
        lastNode -> Occupied = 1;
        return (void*) (lastNode -> next);
    }
    
    struct memNode* tmp = FreeNode -> next;
    struct memNode* NewBlock = (struct memNode*) ((void*)FreeNode + FreeNode -> Size - size);
    NewBlock -> prev = FreeNode;
    NewBlock -> next = tmp;
    FreeNode -> next = NewBlock;
    if(tmp != NULL) {
        tmp -> prev = NewBlock;
    }
    NewBlock -> Size = FreeNode -> Size - size + sizeof(struct memNode*);
    NewBlock -> Occupied = 0;
    FreeNode -> Occupied = 1;
    FreeNode -> Size = size + sizeof(struct memNode*);
    return (void*)FreeNode;// + sizeof(struct memNode*);
}

void OurFree(void* Pointer) {
    struct memNode* ToDelete = (struct memNode*) (Pointer - sizeof(struct memNode*));
    ToDelete -> Occupied = 0;
    if(ToDelete -> next != NULL && ToDelete -> next -> Occupied == 0) {
        ToDelete -> Size += sizeof(struct memNode*) + ToDelete -> next -> Size;
        struct memNode* tmp = ToDelete -> next -> next;
        ToDelete -> next = tmp;
        if(tmp != NULL) {
            tmp -> prev = ToDelete;
        }
    }
    if(ToDelete -> prev != NULL && ToDelete -> prev -> Occupied == 0) {
        ToDelete -> prev -> Size += sizeof(struct memNode*) + ToDelete -> Size;
        struct memNode* tmp = ToDelete -> next;
        ToDelete -> prev -> next = tmp;
        if(tmp != NULL) {
            tmp -> prev = ToDelete -> prev;
        }
    }
}

void* OurRealloc(void* Pointer, int NewSize) {
    struct memNode* ToRealloc = (struct memNode*) (Pointer - sizeof(struct memNode*));
    if (ToRealloc -> next -> Occupied == 0 && ToRealloc -> Size + ToRealloc -> next -> Size >= NewSize) {
        ToRealloc -> Size = NewSize;
        ToRealloc -> next -> Size -= NewSize - ToRealloc -> Size;
        return ToRealloc;
    }
    else {
        OurFree(ToRealloc);
        return OurMalloc(NewSize);
    }
}

int main(int argc, const char * argv[]) {
    int size;
    scanf("%d", &size);
    void* MyAloccated = OurMalloc(size);
    OurRealloc((struct memNode*) OurMalloc(size + 123), 259);
    printf("Hello, World!\n");
    return 0;
}
