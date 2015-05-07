#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    in_addr_t sa1;
    in_addr_t sa2;
    
    in_addr_t mask;
    
    sa1 = inet_addr(argv[1]);
    sa2 = inet_addr(argv[2]);
    mask = inet_addr(argv[3]);
    
    if ((sa1 & mask) == (sa2 & mask))
    {
        printf("1\n");
        exit(1);
    }
    printf("0\n");
    return 0;
}