#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define PAGESIZE 1024

struct memoryCell{
    struct memoryCell* next;
    struct memoryCell* prev;
    size_t size;
    size_t free;
};

struct memoryCell* firstCell;

void firstAlloc(){
    firstCell = (struct memoryCell*)sbrk(1024);
    firstCell->size = 1024 - sizeof(struct memoryCell);
    firstCell->free = 0;
    firstCell->prev = NULL;
    firstCell->next = NULL;
}

struct memoryCell* getLast(){
    struct memoryCell* lastCell = firstCell;
    while (lastCell->next)
        lastCell = lastCell->next;
    return lastCell;
}

void getMemory(struct memoryCell* cell){
    cell->next = (struct memoryCell*)sbrk(1024);
    cell->next->prev = cell;
    cell = cell->next;
    cell->free = 0;
    cell->size = 1024 - sizeof(struct memoryCell);
    cell->next = NULL;
}

struct memoryCell* getTargetCell(size_t size){
    struct memoryCell* targetCell = firstCell;
    while(1){
        if (!targetCell || (targetCell->free>0 && size+sizeof(struct memoryCell)<=targetCell->size))
            break;
        targetCell = targetCell->next;
    }
    return targetCell;
}

void prepareAllocation(size_t size, struct memoryCell* cell, void* position){
    cell->next = (struct memoryCell*)position;
    cell->next->next = NULL;
    cell->next->prev = cell;
    cell->next->size = size;
    cell->next->free = 0;
}

void* my_malloc(size_t size){
    
    if (!firstCell){
        firstAlloc();
        return ( (void*)firstCell + sizeof(struct memoryCell) );
    }
    struct memoryCell* lastCell = getLast();
    struct memoryCell* targetCell = getTargetCell(size);
    
    //get more memory
    if (!targetCell){
        getMemory(lastCell);
        targetCell = getTargetCell(size);
    }
    
    //no free memory
    if (!targetCell)
        return NULL;
    
    void* position = (void*)targetCell + targetCell->size - size;
    prepareAllocation(size, lastCell, position);
    targetCell->size -= (sizeof(struct memoryCell) + size);
    
    return ( (void*)(lastCell->next) + sizeof(struct memoryCell) );
}

void my_free(void* cell){
    struct memoryCell* deleted = (struct memoryCell*)(cell - sizeof(struct memoryCell));
    deleted->free = deleted->size;
    if (deleted->prev && deleted->next){
        if (deleted->prev->size==deleted->prev->free && deleted->next->size==deleted->next->free){
            deleted->prev->next = deleted->next->next;
            deleted->prev->size += (deleted->size+deleted->next->size+2*(sizeof(struct memoryCell)));
            deleted->prev->free = deleted->prev->size;
        }else
        if (deleted->prev->size == deleted->prev->free){
            deleted->prev->size += (deleted->size + sizeof(struct memoryCell));
            deleted->prev->free = deleted->prev->size;
            deleted->prev->next = deleted->next;
            deleted->next->prev = deleted->prev;
        }else
        if (deleted->next->size == deleted->next->free){
            deleted->next->size += (deleted->size + sizeof(struct memoryCell));
            deleted->next->free = deleted->next->size;
            deleted->next->prev = deleted->prev;
            deleted->prev->next = deleted->next;
        }
    }
}

int main(int argc, char** argv)
{
    char* checkString = (char*)my_malloc(255);
    strcpy(checkString, "hello world");
    printf("%s", checkString);
    my_free(checkString);
    return 0;
}