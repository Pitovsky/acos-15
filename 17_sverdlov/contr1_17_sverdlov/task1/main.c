#include <stdio.h>

unsigned short int convert(unsigned int* x) 
{
    union {
        char a;
        int b;
    } t;

    t.b = 1;
    int not_little_endian = (t.a == 0);

    return (unsigned short int) *((unsigned short int*)&x + not_little_endian);
}

int main()
{
    unsigned int x = 256*256 + 76;
    printf("%hu", convert(x));

    return 0;
}


