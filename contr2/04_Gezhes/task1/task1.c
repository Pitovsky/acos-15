#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int ip1;
    int ip2;
    struct sockaddr_in addr1;
    struct sockaddr_in addr2;
    struct sockaddr_in maskaddr;
    int mask;
    if (argc != 4 || inet_aton(argv[1], &addr1.sin_addr) == 0 || inet_aton(argv[2], &addr2.sin_addr) == 0
                  || inet_aton(argv[3], &maskaddr.sin_addr) == 0)
    {
        fprintf(stderr, "Usage: ip ip ip\n");
        return 1;
    }
    ip1 = addr1.sin_addr.s_addr;
    ip2 = addr2.sin_addr.s_addr;
    mask = maskaddr.sin_addr.s_addr;
    if ((ip1 & mask) == (ip2 & mask))
        printf("1\n");
    else
        printf("0\n");

    return 0;
}
