#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <netdb.h>

int main(int argc, char** argv)
{
    struct sockaddr_in addr;
    int addr1 = inet_addr(argv[1]);
    int addr2 = inet_addr(argv[2]);
    int mask = inet_addr(argv[3]);
    int i, j;

    if ((addr1 & mask) && (addr2 & mask))
    {
        printf("1\n");
    }
    else
    {
        printf("0\n");
    }

    /*int* p_addr1 = &addr1;
    int* p_addr2 = &addr2;
    int* p_mask = &mask;

    char* cp1 = (char*)p_addr1;
    char* cp2 = (char*)p_addr2;
    char* cpmask = (char*)mask;
    int len = sizeof(int)*8;

    for (i = 0; i < len; i += 8 )
    {
        for (j = 7; j >= 0; --j)
        {
            if (*cpmask & (1 << j))
            {
                if ((*cp2 & (1 << j))&& (*cp2 & (1 << j)))
                {
                    continue;
                }
                else
                {
                        printf("0\n");
                        return 1;
                }
            }
        }
        ++cpmask;
        ++cp1;
        ++cp2;
    }
    printf("1\n");*/
   return 0;

}
