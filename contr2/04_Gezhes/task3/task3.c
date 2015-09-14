#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int sendall(int sfd, const void *buf, int len, int flags)
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

int main(int argc, char** argv)
{
    struct sockaddr_in servaddr;
    if (argc != 4 || inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        fprintf(stderr, "Usage: ip port file\n");
        return 1;
    }
    FILE* fin = fopen(argv[3], "r");
    if (fin == NULL)
    {
        fprintf(stderr, "Cannot open this file\n");
        return 2;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    int port = atoi(argv[2]);
    servaddr.sin_port = htons(port);

    int pos = 0;
    int strnmb = 0;
    while (1)
    {
        char str[512];
        int i = -1;
        int thiseof = 0;
        while (i < 0 || str[i] != '\n')
        {
            ++i;
            if (fread(str + i, sizeof(char), 1, fin) != 1)
            {
                thiseof = 1;
                break;
            }
        }
        str[i] = 0;
        connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//предполагалось, что это нужно делать один раз. идея: проверить корректность работы некоторого сетевого объекта
        sendall(sockfd, str, strlen(str), 0);
        if (thiseof == 1)
        {
            printf("Success!\n");
            break;
        }
        char nextstr[512];
        i = -1;
        while (i < 0 || nextstr[i] != '\n')
        {
            ++i;
            if (fread(nextstr + i, sizeof(char), 1, fin) != 1)
            {
                thiseof = 1;
                break;
            }
        }
        nextstr[i] = 0;
        connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//?? в уже занятый сокет
        if (recvall(sockfd, str, i, 0) < 0)
        {
            fprintf(stderr, "connection lost!\n");
            return 2;
        }
        str[i] = 0;
        if (strcmp(nextstr, str) != 0)
        {
            //printf("%s != %s\n", str, nextstr);
            fprintf(stderr, "Error: strings in different!\n");
            close(sockfd);
            return 0;
        }
        else
            printf("%d\n", strnmb);
        if (thiseof == 1)
        {
            printf("Success!\n");
            break;
        }
        ++strnmb;
    }
    close(sockfd);
    return 0;
}
