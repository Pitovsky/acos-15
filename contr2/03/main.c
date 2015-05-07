#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char server_reply[2000];
    
    if ((sock = socket(AF_INET , SOCK_STREAM , 0)) < 0)
    {
        printf("Could not create socket");
    }
    
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    int fd = 0;
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Can't open a file");
        exit(1);
    }
    
    struct stat fileInfo;
    lstat(argv[1], &fileInfo);
    const char *data;
    if ((data = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("Can't mmap a file");
        exit(1);
    }
    
    const char *copyOfData = data;
    while(copyOfData - data <= fileInfo.st_size)
    {
        int index = 0;
        for (; copyOfData[index] && copyOfData[index] != '\n'; index++)
        { }
        
        int qty = 0;
        while (qty < index + 1)
        {
            int buff = 0;
            if ((buff = (int)send(sock, copyOfData , index + 1, 0)) < 0)
            {
                puts("Send failed");
                return 1;
            }
            qty += buff;
        }
        
        if(recv(sock, server_reply, 2000, 0) < 0)
        {
            puts("recv failed");
            break;
        }
        
        copyOfData += index + 2;
        
        if (strncmp(server_reply, copyOfData, strlen(server_reply)))
        {
            puts("error!\n");
            exit(EXIT_FAILURE);
        }
        
        copyOfData += strlen(server_reply) + 1;
    }
    
    close(sock);
    munmap((void *)data, fileInfo.st_size);
    close(fd);
    return 0;
}