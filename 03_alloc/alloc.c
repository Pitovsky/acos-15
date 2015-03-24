#include "alloc.h"

#include <unistd.h>
#include <string.h>

#define BLOCK_SIZE 1024

#ifndef DLog
#if !defined(NDEBUG) && LOG
#   define DLog(fmt, ...) printf(("ALLOC: " fmt), ##__VA_ARGS__, ("\n"));
#else
#   define DLog(...)
#endif
#endif

typedef struct
{
    
    short int locked;
    NSUInteger length;
    
} Block;

typedef struct
{
    
    NSUInteger locked;
    NSUInteger maxContiniousFreeBlock;
    NSUInteger freeBlocksQty;
    
} BlockOfData;

void _createNewBlockOfData()
{
    void *ptr = sbrk(BLOCK_SIZE);
    BlockOfData *blockData = ptr;
    
    DLog("New block adress : %p\n", ptr);
    
    memset(blockData, 0, sizeof(BlockOfData));
    
    blockData->freeBlocksQty = 1;
    blockData->maxContiniousFreeBlock = BLOCK_SIZE - sizeof(BlockOfData) - sizeof(Block);
    
    Block *block = ptr + sizeof(BlockOfData);
    block->length = blockData->maxContiniousFreeBlock;
    block->locked = 0;
}

void *alloc(NSUInteger size)
{
    DLog("Started allocating %d bytes ptr\n", size);
    static NSInteger nBlocks = 0;
    
    DLog("NBlocks : %d\n", nBlocks);
    
    if (!nBlocks)
    {
        _createNewBlockOfData();
        nBlocks++;
    }
    
    void *ptr = sbrk(0) - nBlocks * BLOCK_SIZE;
    for (int i = 0; i < nBlocks; i++, ptr += BLOCK_SIZE)
    {
        BlockOfData *blockOfData = (BlockOfData *)ptr;
        
        if (size <= blockOfData->maxContiniousFreeBlock)
        {
            for (void *seeker = ptr + sizeof(BlockOfData); seeker < ptr + BLOCK_SIZE; seeker += ((Block *)seeker)->length + sizeof(Block))
            {
                if (!((Block *)seeker)->locked && size <= ((Block *)seeker)->length)
                {
                    ((Block *)(seeker + sizeof(Block) + size))->locked = 0;
                    ((Block *)(seeker + sizeof(Block) + size))->length = ((Block *)seeker)->length - size - sizeof(Block);
                    ((Block *)seeker)->locked = 1;
                    ((Block *)seeker)->length = size;
                    return seeker + sizeof(Block);
                }
            }
        }
    }
    
}

void *realloc(void *ptr, NSUInteger size)
{
    
}

void free(void *ptr)
{
    
}