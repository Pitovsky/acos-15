#include "stdio.h"

unsigned short int* convert(unsigned int* a)
{
    unsigned short int *r = a;
    return r;
}

int main()
{
    int m;
    scanf("%u", &m);
    printf("answer: %d\n", (int)(*convert(&m)));

    return 0;
}
