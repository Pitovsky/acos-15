#include <stdio.h>

unsigned short int *convert(unsigned int *i)
{
    int check[1];
    *check = 1;
    
    if(*((char *)check) == 1)
    {
        return (unsigned short int *)i;
    }
    else
    {
        return (unsigned short int *)i + 1;
    }
}

int main()
{
    unsigned int a = 65556;
    unsigned short int *ai;
    ai = convert(&a);
    printf("%hu\n", *ai);
    
    return 0;
}
