#include "alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1024
#endif

#define LOG 1
#ifndef DLog
#if !defined(NDEBUG) && LOG
#   define DLog(fmt, ...) printf(("ALLOC: " fmt), ##__VA_ARGS__, ("\n"));
#else
#   define DLog(...)
#endif
#endif

typedef struct
{
    
    char locked;
    unsigned short length;
    
} Block;

void _createNewBlockOfData(int *nBlocks)
{
    void *ptr = sbrk(BLOCK_SIZE);
    if (ptr == NULL)
    {
        perror("Serious allocation error");
        exit(1);
    }
    
    DLog("New block adress : %p\n", ptr);
    
    memset(ptr, 0, BLOCK_SIZE);
    
    Block *block = (Block *)ptr;
    block->length = BLOCK_SIZE - sizeof(Block);
    block->locked = 0;
    
    *nBlocks += 1;
}

void *alloc(NSUInteger size)
{
    if (size > BLOCK_SIZE - sizeof(Block))
    {
        perror("Can't allloc such piece fo data, define BLOCK_SIZE");
        return NULL;
        
    }
    
    DLog("Started allocating %d bytes ptr\n", size);
    static NSInteger nBlocks = 0;
    
    DLog("NBlocks : %d\n", nBlocks);
    
    if (!nBlocks)
    {
        _createNewBlockOfData(&nBlocks);
    }
    
    void *ptr = sbrk(0) - nBlocks * BLOCK_SIZE;
    for (int i = 0; i < nBlocks; i++, ptr += BLOCK_SIZE)
    {
        for (char *seeker = ptr; seeker < ptr + BLOCK_SIZE ; seeker += ((Block *)seeker)->length + sizeof(Block))
        {
            if (!((Block *)seeker)->locked && size + sizeof(Block) <= ((Block *)seeker)->length)
            {
                if (!(size == ((Block *)seeker)->length))
                {
                    ((Block *)(seeker + sizeof(Block) + size))->locked = 0;
                    ((Block *)(seeker + sizeof(Block) + size))->length = ((Block *)seeker)->length - size - sizeof(Block);
                    ((Block *)seeker)->length = size;
                }
                ((Block *)seeker)->locked = 1;
                return seeker + sizeof(Block);
            }
        }
    }
    
    _createNewBlockOfData(&nBlocks);
    return alloc(size);
}

void repairForward(Block *block)
{
    Block *nextBlock = block + sizeof(block) + block->length;
    
    if (!nextBlock->locked && ((char *)sbrk(0) - (char *)nextBlock) % BLOCK_SIZE != 0)
    {
        block->length += sizeof(Block) + nextBlock->length;
        nextBlock->length = 0;
    }
}

void repairBackward(Block *block)
{
    if (((char *)block - (char *)sbrk(0)) % BLOCK_SIZE == 0)
    {
        return;
    }
    
    char *bigBlock = sbrk(0);
    while ((char *)bigBlock - (char *)block > 0)
    {
        bigBlock -= BLOCK_SIZE;
    }
    
    DLog("BACKWARD : BIGBLOCK - %p\n", bigBlock);
    DLog("BACKWARD : recieved block in %p\n", block);
    
    char *prevBlock = (Block *)bigBlock;
    while (((char *)prevBlock + sizeof(Block) + ((Block *)(prevBlock))->length) != (char *)block)
    {
//        for (int i = 0; i < 3; i++)
//        {
            DLog("BACKWARD : STEP %p\n", prevBlock);
            prevBlock = (char *)prevBlock + (sizeof(Block) + ((Block *)(prevBlock))->length);
            if ((char *)prevBlock == (char *)block)
            {
                printf("It's a trap\n");
                break;
            }
//        }
//        break;
    }
    
    if (!((Block *)(prevBlock))->locked)
        repairForward(prevBlock);
}

void free(void *ptr)
{
    Block *block = ptr - sizeof(Block);
    block->locked = 0;
    
    repairForward(block);
    repairBackward(block);
}

void *customRealloc(void *ptr, NSUInteger size)
{
    free(ptr);
    return alloc(size);
}
