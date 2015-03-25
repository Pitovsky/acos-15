#include <stdio.h>
#include <unistd.h>

#include "alloc.h"

#define printPtr(x, y, z) printf("BYTES %s : %d\n", x, (y) - (z));

int main()
{
    void *a = customAlloc(4);
    void *b = customAlloc(10);
    
    void *beginning = sbrk(0);
    beginning -= 1024;
    
    printPtr("a", a, beginning);
    printPtr("b", b, beginning);
    
    a = customRealloc(a, 5);
    printPtr("a", a, beginning);
    customFree(b);
    
    a = customRealloc(a, 5);
    printPtr("a", a, beginning);
    
    return 0;
}