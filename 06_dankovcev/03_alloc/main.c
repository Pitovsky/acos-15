#include <stdio.h>
#include "my-allocator.h"

int main()
{
    void *a1 = my_malloc(10);
    void *a2 = my_malloc(100);
    void *a3 = my_malloc(400);
    void *a4 = my_malloc(12);
    my_free(a1);
    my_free(a3);
    my_free(a2);
    my_realloc(a4, 100);
    void *a11 = my_malloc(1);
    void *a21 = my_calloc(500);
    my_free(a4);
    my_free(a11);
    my_free(a21);
    return 0;
}
