#ifndef MY_ALLOC_H
#define MY_ALLOC_H

void* my_malloc(size_t size);
void* my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

#endif
