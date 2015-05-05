#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sendall(int sfd, char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
    }

    return total;
}
int recvall(int sfd, void* buf, int len, int flags)
{
    int total = 0;
    while(total < len)
    {
        int n = recv(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
    }

    return total;
}

struct sockaddr_in thisAddr;
int sockfd;
char getBuf[512];

int maxUsers = 100;
int users[100];

static void *oneUserListen(void* usrid)
{
    int userID = *((int*)usrid);
    printf("id %d connected\n", userID);

    while (1)
    {
        //connect(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr));
        int msglen;
        if (recv(users[userID], (void*)&msglen, sizeof(int), 0) <= 0)
        {
            printf("id %d disconnected\n", userID);
            break;
        }
        recvall(users[userID], getBuf, msglen, 0);
        getBuf[msglen] = 0;
        printf("id %d send: %s\n", userID, getBuf);
        int i;
        for (i = 0; i < maxUsers; ++i)
            if (i != userID && users[userID] != -1)
        {
            sendall(users[i], getBuf, msglen, 0);
        }
    }
    pthread_exit(NULL);
}

static void *gettingMsgs(void* args)
{
    while(1)
        {
            connect(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr));
            recv(sockfd, getBuf, 512, 0);
            printf("get:%s\n", getBuf);
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

        int i = 0;
        for(i = 0; i < maxUsers; ++i)
            users[i] = -1;

        printf("***E-chat server has correctly started.***\n");
        while(1)
        {
            int newSock = accept(sockfd, NULL, NULL);
            int newUserID = 0;
            while (newUserID < maxUsers && users[newUserID] != -1)
                ++newUserID;
            if (newUserID == maxUsers)
            {
                printf("New connection aborted: Limit of user count.\n");
                continue;
            }
            users[newUserID] = newSock;
            pthread_t listenThr;
            pthread_create(&listenThr, NULL, oneUserListen, (void*)&newUserID);
        }
    }
    else
    {
        printf("***Hello in e-chat - a very simple chat!***\n");
        //thisAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        pthread_t gettingThr;
        pthread_create(&gettingThr, NULL, gettingMsgs, NULL);
        char newStr[512];
        while (1)
        {
            int len = 0;
            char ch = getchar();
            while (len < 512 && ch != '\n' && ch != EOF)
            {
                newStr[len] = ch;
                ++len;
                ch = getchar();
            }
            newStr[len] = 0;
            if (strcmp(newStr, "/exit") == 0)
                break;
            else if (newStr[0] != '/' && newStr[0] != 0)
            {
                //printf("you send:%s\n", newStr);
                connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
                sendall(sockfd, (void*)&len, sizeof(int), 0);
                sendall(sockfd, newStr, len, 0);
            }
            else if (newStr[0] == '/')
                printf("%s: command not found\n", newStr);
        }
        printf("please wait...\n");
        pthread_cancel(gettingThr);
        pthread_join(gettingThr, NULL);
    }

    close(sockfd);

    return 0;
}
