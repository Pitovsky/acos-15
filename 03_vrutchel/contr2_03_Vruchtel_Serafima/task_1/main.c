#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    char *addr1 = (char*)malloc(16);//16 -максимальный размер считываемого адреса
    char *addr2 = (char*)malloc(16);
    char *mask = (char*)malloc(16);

    strcpy(addr1, argv[1]);
    strcpy(addr2, argv[2]);
    strcpy(mask, argv[3]);

    int addr1_int, addr2_int, mask_int;
    addr1_int = inet_addr(addr1);
    addr2_int = inet_addr(addr2);
    mask_int = inet_addr(mask);

    if((addr1_int & mask_int) == (addr2_int & mask_int)) {
        printf("1\n");
        return 1;
    }

    return 0;
}
