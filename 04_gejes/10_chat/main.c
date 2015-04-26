#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int sendall(int s, char *buf, int len, int flags)
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
char getBuf[512];

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

/*static void *oneUser(void* args)
{
    int sockret = *(int*)args;
    while(1)
            {
                char newGetBuf[512];
                int readed = recv(sockret, newGetBuf, 512, 0);
                if(readed > 0)
                {
                    strcpy(getBuf, newGetBuf);
                    printf("get:%s\n", newGetBuf);
                    send(sockret, getBuf, readed, 0);
                }
                else
                {
                    break;
                }
            }
    pthread_exit(NULL);
}*/

int main(int argc, char** argv)
{
    if (argc < 3 || (strcmp(argv[1], "client") != 0 && strcmp(argv[1], "server") != 0)
                 || (strcmp(argv[1], "client") == 0 && (argc < 4 || inet_aton(argv[3], &(thisAddr.sin_addr)) == 0)))
    {
        fprintf(stderr, "Usage: %s client|server port [ip]\n", argv[0]);
        return 1;
    }
    char* mode = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    thisAddr.sin_family = AF_INET;
    thisAddr.sin_port = htons(atoi(argv[2]));

    if (strcmp(mode, "server") == 0)
    {
        //thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
        listen(sockfd, 50);


        while(1)
        {
            int sockret = accept(sockfd, NULL, NULL);
            printf("new socket!\n");
            while(1)
            {
                char newGetBuf[512];
                int readed = recv(sockret, newGetBuf, 512, 0);
                if(readed > 0)
                {
                    strcpy(getBuf, newGetBuf);
                    printf("get:%s\n", newGetBuf);
                    send(sockret, getBuf, readed, 0);
                }
                else
                {
                    break;
                }
            }

            close(sockret);
        }
    }
    else
    {
        printf("ip in addr formate: %d\n", (int)(thisAddr.sin_addr.s_addr));
       // thisAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        pthread_t gettingThr;
        pthread_create(&gettingThr, NULL, gettingMsgs, NULL);
        char newStr[512];
        while (1)
        {
            scanf("%s", newStr);
            if (strcmp(newStr, "/exit") == 0)
                break;
            //printf("you send:%s\n", newStr);

            connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
            sendall(sockfd, newStr, sizeof(newStr), 0);
        }
        printf("please wait...\n");
        pthread_cancel(gettingThr);
        pthread_join(gettingThr, NULL);
    }


    close(sockfd);

    return 0;
}
