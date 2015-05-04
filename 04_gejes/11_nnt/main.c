#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sendall(int s, void *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(s, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
    }

    return total;
}

struct sockaddr_in thisAddr;
int sockfd;
int maxFileSize = 1024*1024;

static void *oneFileGet(void* sockfrom)
{
    pthread_t sockfd = *((pthread_t*)sockfrom);
    int namelen = 0;
    char filename[255];
    int filesize = 0;
    char infile[maxFileSize];
    while(1)
    {
        connect(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr));
        if (recv(sockfd, (void*)&namelen, sizeof(int), 0) <= 0)
        {
            printf("one user disconnected!\n");
            break;
        }
        recv(sockfd, filename, namelen, 0);
        filename[namelen] = 0;
        printf("added file: %s\n", filename);
        recv(sockfd, (void*)&filesize, sizeof(int), 0);
        int i;
        for (i = 0; i < filesize; ++i)
            while (recv(sockfd, infile + i, 1, 0) != 1); //for recv all bytes
        printf("(loaded)\n");

        FILE* newFile = fopen(filename, "wb");
        fwrite(infile, sizeof(char), filesize, newFile);
        fclose(newFile);
    }
    pthread_exit(NULL);
}


int main(int argc, char** argv)
{
    if (argc < 3 || (strcmp(argv[1], "client") != 0 && strcmp(argv[1], "server") != 0)
                 || (strcmp(argv[1], "client") == 0 && (argc < 4 || inet_aton(argv[3], &(thisAddr.sin_addr)) == 0)))
    {
        fprintf(stderr, "Usage: %s <client|server> <port> [ip]\n", argv[0]);
        return 1;
    }
    char* mode = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    thisAddr.sin_family = AF_INET;
    thisAddr.sin_port = htons(atoi(argv[2]));

    if (strcmp(mode, "server") == 0)
    {
        thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
        listen(sockfd, 10);

        printf("server OK!\n");
        while(1)
        {
            int sockFrom;
            sockFrom = accept(sockfd, NULL, NULL);
            printf("new user connected!\n");

            pthread_t oneGetThr;
            pthread_create(&oneGetThr, NULL, oneFileGet, (void*)&sockFrom);
        }
    }
    else
    {
        //thisAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        char com[15];
        while (1)
        {
            scanf("%s", com);
            if (strcmp(com, "exit") == 0)
                break;
            else if (strcmp(com, "add") == 0)
            {
                char name[255];
                char infile[maxFileSize];
                scanf("%s", name);
                FILE* sendFile = fopen(name, "rb");
                if (sendFile == NULL)
                {
                    printf("Error: cannot read this file: %s.\n", name);
                    continue;
                }
                int filesize = 0;
                while (filesize < maxFileSize && fread(infile + filesize, sizeof(char), 1, sendFile) == 1)
                    ++filesize;
                if (filesize == maxFileSize)
                    printf("File too big, cutted by %d bytes.\n", filesize);
                fclose(sendFile);

                connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
                int namelen = strlen(name);
                sendall(sockfd, (void*)&namelen, sizeof(int), 0);
                sendall(sockfd, name, sizeof(char)*strlen(name), 0);
                sendall(sockfd, (void*)&filesize, sizeof(int), 0);
                sendall(sockfd, infile, filesize, 0);
                printf("file successful sended.\n");
            }
            else
                printf("%s: command not found\n", com);
        }
    }

    close(sockfd);

    return 0;
}
