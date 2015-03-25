#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED

#define MY_ALLOC_PIECE_SIZE (1024*1024)

#include <alloca.h>
#include <unistd.h>
#include <string.h>
#include "alloc_int.h"

// резервирует память
void * my_malloc(unsigned int size);

// освобождает память
void my_free(void * mem);

// перемещает память
void * my_realloc(void * mem, unsigned int size);

#endif // ALLOC_H_INCLUDED
