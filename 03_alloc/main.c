#include <stdio.h>
#include <unistd.h>

#include "alloc.h"

int main()
{
    int *a = alloc(4);
    
    printf("%p\n", a);
    
    a = alloc(4);
    
    printf("%p\n", a);
    
    return 0;
}