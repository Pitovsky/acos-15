#include <stdio.h>
#include <unistd.h>

#include "alloc.h"

#define printPtr(x, y) printf("BYTES %s : %p\n", x, (y));

int main()
{
    void *a = alloc(4);
    void *b = alloc(4);
    printPtr("allocate a", a);
    printPtr("allocate b", b);
    
    a = customRealloc(a, 5);
    printPtr("realloc a", a);
    free(b);
    printf("Freed b\n");
    
    a = customRealloc(a, 4);
    printPtr("a", a);
    
    free(a);
    
    return 0;
}