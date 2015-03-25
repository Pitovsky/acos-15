#include <unistd.h>
#include <stdio.h>
#include <string.h>

static const unsigned DEFAULT_MEM_REQUEST = 1 << 20;

void *my_malloc(unsigned space_amount);
void my_free(void *ptr);
void *my_calloc(unsigned space_amount);
void *my_realloc(void *ptr, unsigned new_space_amount);
