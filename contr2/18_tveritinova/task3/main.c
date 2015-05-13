#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char** argv)
{
    int fd = open(argv[1], O_RDWR);
    int port = atoi(argv[2]);
    char* ip = argv[3];
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        perror("connect");
    }
    
    char stream;
    int er_read = read(fd, &stream, 1);
    int key  = 1;// 1 - нечет, 0 - чет
    int string_to_server[1024];
    int string_to_client[1024];
    int counter_string_server = 0;
    int counter_string_client = 0;
    int string_count = 0;
    while (er_read != -1)//цикл то бесконечный
    {
        if (key == 1)
        {
            string_to_server[counter_string_server] = stream;
            ++counter_string_server;
        }
        if (key == 0)
        {
            string_to_client[counter_string_client] = stream;
            ++counter_string_client;
        }
        if ((stream == '\n') && (key == 0))
        {
            ++string_count;
            string_to_client[counter_string_client] = 0;
            if (strcmp(string_to_server, string_to_client) != 0)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("%d\n", string_count / 2);
            }
            counter_string_client = 0;
            key = 1;
        }
        if ((stream == '\n') && (key == 1))
        {
            ++string_count;
            string_to_server[counter_string_server] = 0;
            write(sock, &string_to_server, counter_string_server + 1);//а если с одного раза не получится?
            read(sock, &string_to_server, counter_string_server + 1);
            counter_string_server = 0;
            key = 0;
            
        }
        read(fd, &stream, 1);//я жутко против посимвольного небуферезованного считывания
    }
    
}
