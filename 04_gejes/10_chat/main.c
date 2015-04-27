#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
        thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
        listen(sockfd, 10);

        int maxUsers = 100;
        int users[maxUsers];
        int i = 0;
        for(i = 0; i < maxUsers; ++i)
            users[i] = 0;

        printf("***E-chat server has correctly started.***\n");
        while(1)
        {
            fd_set foread;
            FD_ZERO(&foread);
            FD_SET(sockfd, &foread);
            int sockFrom;
            int nextfd = sockfd;
            for(i = 0; i < maxUsers; ++i)
            {
                if(users[i] != 0)
                {
                    FD_SET(users[i], &foread);
                }
                if(nextfd < users[i])
                    nextfd = users[i];
            }

            int code = select(nextfd + 1, &foread, NULL, NULL, NULL);
            if (code < 0)
            {
                perror("select error\n");
            }

            if (FD_ISSET(sockfd, &foread))
            {
                sockFrom = accept(sockfd, NULL, NULL);

                i = 0;
                while (i < maxUsers && users[i] != 0)
                    ++i;
                printf("New user (id %d) connected\n", i);

                if(i == maxUsers)
                {
                    fprintf(stderr, "Error: user limit exceeded!\n");
                    return 1;
                }

                users[i] = sockFrom;
            }

            for(i = 0; i < maxUsers; ++i)
            {
                if(users[i] != 0 && FD_ISSET(users[i], &foread))
                {
                    int readed = recv(users[i], getBuf, 512, 0);
                    if(readed <= 0)
                    {
                        close(users[i]);
                        users[i] = 0;
                        printf("id %d disconnected.\n", i);
                        continue;
                    }
                    printf("id %d send: %s\n", i, getBuf);
                    int j = 0;
                    for (j = 0; j < maxUsers; ++j)
                        if (users[j] != 0 && j != i) //sending to all instead i
                            send(users[j], getBuf, readed, 0);
                }
            }
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
            int i = 0;
            char ch = getchar();
            while (i < 512 && ch != '\n' && ch != EOF)
            {
                newStr[i] = ch;
                ++i;
                ch = getchar();
            }
            newStr[i] = 0;
            if (strcmp(newStr, "/exit") == 0)
                break;
            else if (newStr[0] != '/' && newStr[0] != 0)
            {
                //printf("you send:%s\n", newStr);
                connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
                sendall(sockfd, newStr, sizeof(newStr), 0);
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
