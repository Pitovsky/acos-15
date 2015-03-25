#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "my_alloc.h"

void *max(void *a, void *b) 
{
    return (a > b ? a : b);
}

static const int MEM_BLOCK_SIZE = 1024*1024; // 1Mb
static const int MEM_MIN_BUFFER_SIZE = 16;


struct mem_buffer_t {
    size_t       size; // whole size with this information
    struct mem_buffer_t *next;
    char         is_free;
};

struct mem_buffer_t *head;
void *last_available_space;
static char *is_initialized;

static void init() 
{
    last_available_space = sbrk(0) + 1;
    sbrk(4 * sizeof(struct mem_buffer_t));

    head = last_available_space;
    head->next = NULL;
    last_available_space += sizeof(struct mem_buffer_t);

    is_initialized = last_available_space;
    last_available_space += sizeof(char);
}



void *my_free(void *ptr) 
{
    // find information about buffer
    struct mem_buffer_t *cur_node;
    struct mem_buffer_t *node;
        
    cur_node = (void*)ptr - sizeof(struct mem_buffer_t);
    assert(cur_node->is_free == 0);
    cur_node->is_free = 1;

    // node -- previous free buffer (or head)
    node = head;
    while (node->next && node->next < cur_node) {
        node = node->next;
    }

    cur_node->next = node->next; // insert cur_node to list
    node->next = cur_node;
}

struct mem_buffer_t *occupy_existing_buffer(size_t size) 
{
    struct mem_buffer_t *node, *prev_node, 
                 *sliced_node = NULL;


    // search for buffer to occupy
    node = head->next;
    while (node && node->size < size) {
        node = node->next;
    }

    // failure
    if (!node) {
        return 0;
    }

    if (node->size - size > sizeof(struct mem_buffer_t) + MEM_MIN_BUFFER_SIZE) {
        // cut
        sliced_node = (void*)node + size;

        sliced_node->is_free = 1;
        sliced_node->size = node->size - size;
        sliced_node->next = node->next;

        // shrink
        node->next = sliced_node;
        node->size = size;
    }
    node->is_free = 0;

    // find previous node in free-list
    prev_node = head;
    while (prev_node->next && prev_node->next < node) {
        node = node->next;
    }

    /*prev_node->next = node->next; // delete node from free-list*/
    // insert right half 
    if (sliced_node) {
        prev_node->next = sliced_node;
    } else {
        prev_node->next = node->next;
    }

    return node;
}

void extend_space_to_fit(size_t size)
{
    size_t buffer_size = MEM_BLOCK_SIZE * 
                        ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    void *res;
    res = sbrk(buffer_size);
    if (res < 0) {
        fprintf(stderr, "Can't get memory");
        _exit(0);
    }

    struct mem_buffer_t *last_node, *node;
    
    node = last_available_space;

    node->size = buffer_size;
    node->next = NULL;
    node->is_free = 1;


    last_node = head;
    while (last_node->next) {
        last_node = last_node->next;
    }
    // insert node at the end of free-list
    last_node->next = node;
}

void* my_malloc(size_t size) 
{
    if (!is_initialized) {
        init();
    }

    size += sizeof(struct mem_buffer_t);

    struct mem_buffer_t *node;
    if (!(node = occupy_existing_buffer(size))) {
        // failure
        extend_space_to_fit(size);
        node = occupy_existing_buffer(size);
    } 
    assert(node > 0);
    last_available_space = max(last_available_space, node + size);
    return (void*)node + sizeof(struct mem_buffer_t);
}

int main() 
{
    int* A = my_malloc(5000000 * sizeof(int));
    my_free(A);
    printf("was address: %d\n", (int)A);

    A = my_malloc(50000 * sizeof(int));
    printf("what will be adress this time?: %d\n", (int)A);

    int* B = my_malloc(54321 * sizeof(int));
    int i;
    for (i = 0; i < 50; ++i)
        B[i] = i;

    my_free(A);
    my_free(B);

    printf("seems to work");


    return 0;
}
