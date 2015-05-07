#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    assert(4 == argc);

    char* address = argv[1];
    int port = atoi(argv[2]);
    FILE* protocolFile = fopen(argv[3], "r");

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(0 <= sock);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return(2);
    }

    while (!feof(protocolFile))
    {
        char *line = 0;
        size_t length = 0;
        assert(getline(&line, &length, protocolFile) != -1);
        length = strlen(line) - 1;
        line[length] = 0;


        size_t current = 0;
        while (current < length)
        {
            size_t advanced = send(sock, line + current, length - current, 0);
            current += advanced;
        }

        free(line);

        length = 0;
        assert(getline(&line, &length, protocolFile) != -1);

        char* buffer = (char*)malloc(length + 2);

        recv(sock, buffer, length, 0);
        buffer[length + 1] = 0;
        printf("%s\n%s\n", line, buffer);
        assert(strcmp(line, buffer) == 0);
        free(buffer);
        free(line);
    }

    fclose(protocolFile);

    return 0;
}
