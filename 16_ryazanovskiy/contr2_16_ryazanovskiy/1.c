#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int sameSubnet(const char* addrString1, const char* addrString2, const char* maskString)
{
    int bytes[4] = {};
    sscanf(addrString1, "%i.%i.%i.%i", bytes, bytes + 1,  bytes + 2,  bytes + 3);
    uint32_t addr1 = bytes[3] + (bytes[2] << 8) + (bytes[1] << 16) + (bytes[0] << 24);

    sscanf(addrString2, "%i.%i.%i.%i", bytes, bytes + 1,  bytes + 2,  bytes + 3);
    uint32_t addr2 = bytes[3] + (bytes[2] << 8) + (bytes[1] << 16) + (bytes[0] << 24);

    sscanf(maskString, "%i.%i.%i.%i", bytes, bytes + 1,  bytes + 2,  bytes + 3);
    uint32_t mask = bytes[3] + (bytes[2] << 8) + (bytes[1] << 16) + (bytes[0] << 24);

    return !!((addr1 & mask) == (addr2 & mask));
}

int main()
{
    char addr1[100] = "", addr2[100] = "", mask[100] = "";

    scanf("%s%s%s", addr1, addr2, mask);

    printf("%i\n", sameSubnet(addr1, addr2, mask));

    return 0;
}
