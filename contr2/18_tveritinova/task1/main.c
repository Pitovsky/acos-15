#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    in_addr_t one = inet_addr(argv[1]);
    in_addr_t two = inet_addr(argv[2]);
    in_addr_t three = inet_addr(argv[3]);
    if ((one & three) == (two & three))
    {
        printf("1\n");
    }
}
