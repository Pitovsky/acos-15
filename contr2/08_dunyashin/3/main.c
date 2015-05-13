#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>

int sendall(int sfd, const void *buf, int len, int flags){
    int sum = 0, n;
    while(sum < len)
    {
        n = send(sfd, buf + sum, len - sum, flags);//есть же ещЄ ноль
        if(n == -1){return -1;}
        sum += n;
    }
    return sum;
}

int recvall(int sfd, void* buf, int len, int flags)
{
    int sum = 0;
    while(sum < len){
        int n = recv(sfd, buf + sum, len - sum, flags);
        if(n == -1)
            return -1;
        sum += n;
    }

    return sum;
}

int main(int argc, char** argv)
{
    struct sockaddr_in serveraddr;
    if (argc != 4 || inet_aton(argv[1], &serveraddr.sin_addr) == 0){
        fprintf(stderr, "Usage: ip port file\n");
        return 1;
    }
    FILE* fin = fopen(argv[3], "r");
    if (fin == NULL){
        fprintf(stderr, "Can't open file\n");
        return 2;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    int port = atoi(argv[2]);
    serveraddr.sin_port = htons(port);
    int pos = 0;
    int strnmb = 0;
    while (1){
        char str[512];
        int i = -1;
        int thiseof = 0;
        while (i < 0 || str[i] != '\n'){
            ++i;
            if (fread(str + i, sizeof(char), 1, fin) != 1){
                thiseof = 1;
                break;
            }
        }
        str[i] = 0;
        connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));//вообще, connect предполагалось делать один раз всего
        sendall(sockfd, str, strlen(str), 0);
        if (thiseof == 1){
            printf("Success!\n");
            break;
        }
        char nextstr[512];
        i = -1;
        while (i < 0 || nextstr[i] != '\n'){//буфер то переполн€ем. стандартна€ у€звимость "атака на переполнение стэка"
            ++i;
            if (fread(nextstr + i, sizeof(char), 1, fin) != 1){
                thiseof = 1;
                break;
            }
        }
        nextstr[i] = 0;
        connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
        if (recvall(sockfd, str, i, 0) < 0){
            fprintf(stderr, "connection lost!\n");
            return 2;
        }
        str[i] = 0;
        if (strcmp(nextstr, str) != 0){
            fprintf(stderr, "Error: strings in different!\n");
            close(sockfd);
            return 0;
        }else{printf("%d\n", strnmb);}
        if (thiseof == 1){
            printf("Success!\n");
            break;
        }
        ++strnmb;
    }
    close(sockfd);
    return 0;
}
