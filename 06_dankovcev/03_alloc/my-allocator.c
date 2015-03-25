#include "my-allocator.h"

static const char IS_FREE = 1;
static const char NOT_FREE = 0;

//16 bytes
typedef struct Header
{
    unsigned block_size;
    char is_free;
    struct Header *next, *prev;
} Header;

Header *in_block = NULL;

void *request_mem(unsigned space_amount)
{
    void *allocated_mem = sbrk(space_amount);

    if (allocated_mem == ((void *) -1))
    {
        fprintf(stderr, "Can't move program break.\n");
        return NULL;
    }
    else
    {
        return allocated_mem;
    }
}

Header *get_last_block(Header *in_block)
{
    Header *current_node = in_block;

    if (in_block == NULL)
        return NULL;

    while (current_node->next != NULL)
        current_node = current_node->next;

    return current_node;
}

void print_mem_list(Header *in_block)
{
    Header *current_block = in_block;

    while (current_block != NULL)
    {
        printf("\033[35m%p - %d(HEADER)\033[0m\n", current_block, sizeof(Header));

        if (current_block->is_free == IS_FREE)
        {
            printf("\033[32m%p - %d(FREE)\033[0m\n\n",
                   (char*)current_block + sizeof(Header),
                   current_block->block_size);
        }
        else
        {
            printf("\033[31m%p - %d(ALLOC)\033[0m\n\n",
                   (char*)current_block + sizeof(Header),
                   current_block->block_size);
        }

        current_block = current_block->next;
    }

    printf("\n");
}


Header *create_new_block()
{
    Header *new_block = request_mem(DEFAULT_MEM_REQUEST);

    if (new_block == NULL)
        return NULL;

    new_block->next = NULL;
    new_block->prev = NULL;
    new_block->is_free = IS_FREE;
    new_block->block_size = DEFAULT_MEM_REQUEST - sizeof(Header);

    return new_block;
}

void merge_next(Header *block)
{
    if (block->next != NULL && block->next->is_free == IS_FREE)
    {
        block->block_size += block->next->block_size +
                             sizeof(Header);

        if (block->next->next != NULL)
            block->next->next->prev = block;

        block->next = block->next->next;
    }
}

void merge_prev(Header *block)
{
    if (block->prev != NULL && block->prev->is_free == IS_FREE)
    {
        block->prev->block_size += block->block_size +
                                   sizeof(Header);

        block->prev->next = block->next;

        if (block->next != NULL)
            block->next->prev = block->prev;
    }
}

void merge_free_blocks(Header *block)
{
    merge_next(block);
    merge_prev(block);
}

Header *extend_last_block(Header *in_block)
{
    Header *last_block = get_last_block(in_block);
    Header *new_block = create_new_block();

    //Can't allocate more
    if (new_block == NULL)
        return NULL;

    //List is empty
    if (last_block == NULL)
        return new_block;

    last_block->next = new_block;
    new_block->prev = last_block;

    if (last_block->is_free == NOT_FREE)
    {
        return new_block;
    }
    else
    {
        merge_next(last_block);
        return last_block;
    }
}

void *allocate_block(Header *block, unsigned space_amount)
{
    if (block->block_size - space_amount > sizeof(Header))
    {
        Header *new_block = (Header*) ((char*) (block) + sizeof(Header) + space_amount);

        new_block->next = block->next;
        new_block->prev = block;
        new_block->block_size = block->block_size - space_amount - sizeof(Header);
        new_block->is_free = IS_FREE;

        if (block->next != NULL)
        {
            block->next->prev = new_block;
        }

        block->next = new_block;
        block->block_size = space_amount;

        merge_next(new_block);
    }

    block->is_free = NOT_FREE;
    print_mem_list(in_block);

    return (char*)block + sizeof(Header);
}

void *my_malloc(unsigned space_amount)
{
    printf("\033[0mALLOC: %d\033[0m\n", space_amount);

    if (in_block == NULL)
    {
        in_block = extend_last_block(in_block);
    }

    Header *current_block = in_block;

    while (current_block != NULL)
    {
        if (current_block->is_free == IS_FREE &&
            current_block->block_size >= space_amount)
        {
            return allocate_block(current_block, space_amount);
        }

        current_block = current_block->next;
    }

    printf("No free memory. Try to request more.\n");

    //Try to extend last block
    Header *last_block = extend_last_block(in_block);

    while (last_block->block_size < space_amount)
    {
        //Can't allocate enough
        if (extend_last_block(in_block) == NULL)
            return NULL;
    }

    return allocate_block(last_block, space_amount);
}

void *my_calloc(unsigned space_amount)
{
    printf("\033[0mCALLOC: %d\033[0m\n", space_amount);

    void *new_mem = my_malloc(space_amount);

    if (new_mem == NULL)
        return NULL;

    return memset(new_mem, 0, space_amount);
}

void *my_realloc(void *ptr, unsigned new_space_amount)
{
    printf("\033[0mREALLOC: %p - %d\033[0m\n", ptr, new_space_amount);
    Header *block = (Header*) ((char*)ptr - sizeof(Header));

    if (new_space_amount <= block->block_size)
    {
        //Cut off rest free space if can
        return allocate_block(block, new_space_amount);
    }
    else
    {
        if (block->next != NULL && block->next->is_free == IS_FREE &&
            (block->block_size + block->next->block_size + sizeof(Header)) >= new_space_amount)
        {
            merge_next(block);
            return allocate_block(block, new_space_amount);
        }
        else
        {
            void *new_mem = my_malloc(new_space_amount);

            if (new_mem == NULL)
                return NULL;

            memcpy(new_mem, ptr, block->block_size);
            my_free(ptr);

            return new_mem;
        }
    }
}

void my_free(void *ptr)
{
    Header *block = (Header*) ((char*)ptr - sizeof(Header));
    printf("\033[0mFREE: %p - %d\033[0m\n", ptr, block->block_size);

    block->is_free = IS_FREE;
    merge_free_blocks(block);

    if (in_block->next == NULL &&
        in_block->prev == NULL &&
        in_block->is_free == IS_FREE)
    {
        printf("Return all memory.\n");
        brk(in_block);
        in_block = NULL;
    }

    print_mem_list(in_block);
}
