#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>

void assertMessage(int assertion, const char* message)
{
    if (!assertion)
        perror(message);
    assert(assertion);
}

int socketConnect(sa_family_t domain, int type, const char* address, uint16_t port)
{
    int sock = socket(domain, type, 0);
    assertMessage(0 <= sock, "can't open socket fd\n");

    struct sockaddr_in addr;
    addr.sin_family = domain;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);
    assertMessage(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) >= 0, "can't connect");
    return sock;
}

#pragma pack(push, 1)
typedef struct
{
    uint16_t id;
    unsigned recursionDesired : 1, truncated : 1, authoritativeAnswer : 1, opcode : 4, queryOrResponse : 1,
             returnCode : 4, checkingDisabled : 1, authenticatedData : 1, Z : 1, recursionAvailable : 1;
    uint16_t questionCount;
    uint16_t answersCount;
    uint16_t authorityCount;
    uint16_t additionalCount;
} DnsMessageHeader;

typedef struct
{
    uint16_t name, type, class_;
    uint32_t TTL;
    uint16_t dataLength;
} DnsResponse;
#pragma pack(pop)

void dnsEncode(char* name)
{
    memmove(name + 1, name, strlen(name) + 1);
    int currLabel = 0;
    int i = 1;
    while (name[i])
    {
        int j = 0;
        for (; name[i + j] && name[i + j] != '.' && j < 256; j++);
        assert(j < 256 && "Domain name is too long");
        name[currLabel] = (char) j;
        currLabel = (i = i + j);
        i++;
    }
}

void writeToSocket(int socket, char* data, size_t length)
{
    size_t current = 0;
    while (current < length)
    {
        size_t advanced = (size_t) send(socket, data + current, length - current, 0);
        current += advanced;
    }
}

void convertHeader(DnsMessageHeader* header)
{
    header->id = htons(header->id);
    header->questionCount = htons(header->questionCount);
    header->answersCount = htons(header->answersCount);
    header->authorityCount = htons(header->authorityCount);
    header->additionalCount = htons(header->additionalCount);

}

void convertResponse(DnsResponse* response)
{
    response->name = htons(response->name);
    response->type = htons(response->type);
    response->class_ = htons(response->class_);
    response->TTL = htonl(response->TTL);
    response->dataLength = htons(response->dataLength);
}

int main(int argc, char** argv)
{
    assert(sizeof(DnsMessageHeader) == 12);
    assert(2 == argc);

    char* dnsServer = argv[1];
    int sock = socketConnect(AF_INET, SOCK_DGRAM, dnsServer, 53);

    char* request = 0;
    size_t bufferLength = 0;

    uint16_t messageId = 0;
    while (messageId++, !feof(stdin))
    {
        DnsMessageHeader message = {};
        message.id = messageId;
        message.queryOrResponse = 0;
        message.recursionDesired = 1;
        message.questionCount = 1;

        convertHeader(&message);

        memset(request, 0, bufferLength);
        if (getline(&request, &bufferLength, stdin) == -1)
            break;
        size_t domainLength = strlen(request);
        request[domainLength - 1] = 0;
        dnsEncode(request);
        domainLength++;

        char buffer[1000];
        memcpy(buffer, &message, sizeof(message));
        memcpy(buffer + sizeof(message), request, domainLength);
        memcpy(buffer + sizeof(message) + domainLength, "\0\x01\0\x01", 4);
        writeToSocket(sock, buffer, sizeof(message) + domainLength + 4);

        recv(sock, buffer, sizeof(buffer) - 1, 0);
        memcpy(&message, buffer, sizeof(message));
        convertHeader(&message);

        if (messageId != message.id || message.returnCode)
        {
            fprintf(stderr, "There was an error: RCODE %i\n", message.returnCode);
            continue;
        }

        assert(0 == strcmp(request, buffer + sizeof(message)));

        if (!message.answersCount)
            fprintf(stderr, "No addresses for this host\n", message.returnCode);
        else
            for (size_t i = 0, currPos = (sizeof(message) + strlen(request) + 5); i < message.answersCount; i++)
            {
                unsigned char host[100] = "";
                DnsResponse response;
                memcpy(&response, buffer + currPos, sizeof(response));
                convertResponse(&response);
                assert(response.name >= 0b1100000000000000);
                memcpy(host, buffer + currPos + sizeof(response), response.dataLength);
                currPos += sizeof(response) + response.dataLength;
                if (1 == response.type)
                {
                    if (4 == response.dataLength)
                        printf("%i.%i.%i.%i\n", (int) host[0], (int) host[1], (int) host[2], (int) host[3]);
                }
            }
    }

    free(request);

    return 0;
}
