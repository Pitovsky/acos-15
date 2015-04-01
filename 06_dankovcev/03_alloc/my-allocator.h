#include <unistd.h>
#include <stdio.h>
#include <string.h>

static const size_t DEFAULT_MEM_REQUEST = 1 << 20;

void *my_malloc(size_t space_amount);
void my_free(void *ptr);
void *my_calloc(size_t space_amount);
void *my_realloc(void *ptr, size_t new_space_amount);
