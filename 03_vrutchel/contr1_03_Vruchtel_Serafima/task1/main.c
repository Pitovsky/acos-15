#include <stdio.h>
#include <stdlib.h>

unsigned short int checker(unsigned int n) {//check little endian or big endian
    return (unsigned short int)n;
}
unsigned short int* convert(unsigned int* a) {
    unsigned short int *b;
    if(checker(1) == 1) {//little endian
        b = (unsigned short int*)a;
        *b = *b % (256 * 256);
    } else { //big endian
        b = a + 2;
    }
    return b;
}

int main()
{
    unsigned int a = 1;
    printf("%d\n", *convert(&a));
    return 0;
}
