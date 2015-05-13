#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <netdb.h>

int main(int argc, char** argv)
{
    int socket_id = 0;
    struct sockaddr_in addr;
    socket_id = socket(AF_INET, SOCK_STREAM, 0 );
    addr.sin_family = AF_INET;
    int port_num = atoi(argv[2]);
    addr.sin_port = htons(port_num);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    while (connect(socket_id, (struct sockaddr*)&addr, sizeof(addr) ))
    {
        printf("please wait\n");
        sleep(1);
    }

    FILE* fd = fopen(argv[3],"r");
    char str1[1000];
    char str2[1000];
    int check = 0;

    while (fgets(str1, sizeof(str1), fd))
    {
        char buf[1000];
        write(socket_id, str1, strlen(str1));//все операции чтения-записи было бы лучше делать под циклом
        read(socket_id, buf, sizeof(buf));
        fgets(str2, strlen(str2), fd);
        if (!strcmp(buf, str2))
        {
            fprintf(stdin, "%d\n", check);
            check += 2;
        }
        else
        {
            perror("error");
            exit(1);
        }
    }
    return 0;
}
