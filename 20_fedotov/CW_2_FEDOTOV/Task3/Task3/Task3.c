//
//  main.c
//  Task3
//
//  Created by Simon Fedotov on 07.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <math.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int ToSend(int SocketFileDesc, char* OurMessage, int MessageSize, int Flags) {
    int Sent = 0;
    
    while (Sent < MessageSize) {
        int BytesNumber = send(SocketFileDesc, OurMessage + Sent, MessageSize - Sent, 0);
        if(BytesNumber < 0)
            return 0;
        
        Sent += BytesNumber;
    }
    
    return Sent;
}

int ToRecieve(int SocketFileDesc, char* OurMessage, int MessageSize, int Flags) {
    int Recieved = 0;
    
    
    while (Recieved < MessageSize) {
        int BytesNumber = recv(SocketFileDesc, OurMessage + Recieved, MessageSize - Recieved, 0);
        if(BytesNumber <= 0)
            return 0;
        
        Recieved += BytesNumber;
    }
    
    return Recieved;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    int PORT;
    char* IP = malloc(255);
    char* FilePath = malloc(1 << 10);
    IP = argv[1];
    PORT = atoi(argv[2]);
    FilePath = argv[3];
    
    int OurFD = open(FilePath, O_RDWR | O_CREAT | S_IRWXU, 0777);
    if(OurFD < 0) {
        perror("Error While opening File");
        exit(1);
    }
    
    struct stat statistic;
    if(fstat(OurFD, &statistic) < 0)
        printf("Can't get stats");
    int FileSize = statistic.st_size;
    
    char* FileAddress = (char*)mmap(NULL, FileSize, PROT_READ | PROT_WRITE, MAP_SHARED, OurFD, 0);
    if(FileAddress == NULL) {
        perror("Error while mapping");
        exit(1);
    }
    
    
    printf("Bubu = %s\n", FileAddress);
    

    
    
    
    int Client;
    struct sockaddr_in ClientAddress;
    
    Client = socket(AF_INET, SOCK_STREAM, 0);
    if(Client < 0){
        perror("Client Socket Error");
        exit(1);
    }
    
    ClientAddress.sin_family = AF_INET;
    ClientAddress.sin_port = htons(PORT);
    ClientAddress.sin_addr.s_addr = inet_addr(IP);
    
    
    
    if(connect(Client, (struct sockaddr *)&ClientAddress, sizeof(ClientAddress)) < 0){
        perror("Client connection Error");
        exit(1);
    }
    
    int Offset = 0;
    int Counter = 0;
    while (1) {
        ++Counter;
        if(Offset >= FileSize) {
            printf("END\n");
            break;
        }
        
        char* First = malloc(1 << 20);
        char* Second = malloc(1 << 20);
        char* Recieved = malloc(1 << 20);
        int i = 0;
        for(i = Offset; FileAddress[i] != '\n' && i < FileSize; ++i) {}
        strncpy(First, FileAddress + Offset, i - Offset + 1);
        Offset = i + 1;
        
        for(i = Offset; FileAddress[i] != '\n' && i < FileSize; ++i) {}
        strncpy(Second, FileAddress + Offset, i - Offset + 1);
        Offset = i + 1;
        
        
        ToSend(Client, First, strlen(First), 0);
        
        printf("First = %s\n", First);

        
        ToRecieve(Client, Recieved, strlen(Second), 0);
        printf("Recieved = %s\n", Recieved);
        
        
        if(!strcmp(Second, Recieved) && Recieved[strlen(Recieved) - 1] != '\n') {
            printf("ERROORR String are NOT THE SAME1\n");
            printf("Result is  =  %d\n", Counter - 1);
            exit(1);
        }
            
        
        else if(strcmp(Second, Recieved)) {
            printf("ERROORR String are NOT THE SAME\n");
            printf("Result is  =  %d\n", Counter - 1);

            exit(1);
        }
        
        printf("Second = %s\n", Second);
        
        memset(First, 0, strlen(First));
        memset(Second, 0, strlen(Second));
        memset(Recieved, 0, strlen(Recieved));
        
    }
    
    
    close(Client);
    
    munmap(FileAddress, FileSize);
    return 0;
}
