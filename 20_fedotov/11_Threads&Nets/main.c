//
//  main.c
//  Chat
//
//  Created by Simon Fedotov on 25.04.15.
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

int HowManyMessages = 10;
int SocketFD;
struct sockaddr_in OurAddress;

struct DatMessage{
    int NameLen;
    int DataSize;
    char Name[1 << 10];
    char Data[1 << 22];
};


int ToSend(int SocketFileDesc, struct DatMessage* OurMessage, int MessageSize, int Flags) {
    int Sent = 0;
    
    while (Sent < MessageSize) {
        int BytesNumber = send(SocketFileDesc, OurMessage + Sent, MessageSize - Sent, 0);
        if(BytesNumber < 0)
            return 0;
        
        Sent += BytesNumber;
    }
    
    return Sent;
}

int ToRecieve(int SocketFileDesc, struct DatMessage* OurMessage, int MessageSize, int Flags) {
    int Recieved = 0;
    
    while (Recieved < MessageSize) {
        int BytesNumber = recv(SocketFileDesc, OurMessage + Recieved, MessageSize - Recieved, 0);
        if(BytesNumber < 0)
            return 0;
        
        Recieved += BytesNumber;
    }
    
    return Recieved;
}

void* ServerJob(int* SocketFromDescr) {
    printf("---------Hello, Thread-----------\n");
    pthread_t SocketFD = *SocketFromDescr;
    struct DatMessage* Message = (struct DatMessage*) malloc(sizeof(struct DatMessage*));
    
    while (1) {
        int Res = connect(SocketFD, (struct sockaddr*)&OurAddress, sizeof(OurAddress));
//        if(Res < 0) {
//            perror("Error while connecting at work");
//            break;
//        }
        
        
        int BytesNumber = ToRecieve(SocketFD, Message,  2 * sizeof(int) + (1 << 10) + (1 << 23), 0);
        if(BytesNumber < 0) {
            perror("Bubu! recv Error");
            exit(1);
        }
        
        if(Message -> DataSize == 0 || Message -> NameLen == 0)
            continue;
        
        printf("_-'-_|_-'-_[Server've got the message!]_-'-_|_-'-_\n_!_!_INFO_!_!_\n");

        printf("- NameLen --> %d\n", Message -> NameLen);
        printf("- Name --> %s\n", Message -> Name);
        printf("- DataSize --> %d \n", Message -> DataSize);
        printf("- Data --> %s\n", Message -> Data);
        
        char* Filename = malloc(1 << 10);
        strcpy(Filename, "/Users/semenfedotov/Threads/");
        strcat(Filename, Message -> Name);
        
        
        printf("FileNaame is = %s\n");
        
        int DatFileFD = open(Filename, O_RDWR | O_CREAT | S_IRWXU, 0777);
        if(DatFileFD < 0) {
            perror("Error while opening Server File");
            exit(1);
        }
        
        long long NumberWritten = 0;
        while (NumberWritten < Message -> DataSize) {
            NumberWritten += write(DatFileFD, Message -> Data + NumberWritten, 4094);
        }
        if(NumberWritten < 0) {
            perror("Error while Writing Data into File");
            exit(1);
        }
        
        close(DatFileFD);
        
        
        free(Filename);
    }
    
    free(Message);
    
    pthread_exit(NULL);
    
    
}
    


int main(int argc, const char * argv[]) {
    setbuf(stdout, 0);
    int Mode;
    scanf("%d", &Mode);
    
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(SocketFD < 0) {
        perror("Error While socketing Client");
        exit(1);
    }
    
    int PORT;
    printf("Please, enter Port Number : ");
    scanf("%d", &PORT);

    
    OurAddress.sin_family = AF_INET;
    OurAddress.sin_port = htons(PORT);

    
    if(Mode == 0) {//Server
        
        OurAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        
        if(bind(SocketFD, (struct sockaddr*)&OurAddress, sizeof(OurAddress)) < 0) {
            perror("Binding Error");
            exit(1);
        }
        
        listen(SocketFD, HowManyMessages);
        
        printf("_-_-_-__-_-_-_Server Has started without errors_-_-_-__-_-_-_\n");
        
        while (1) {
            int Server = accept(SocketFD,NULL, NULL);
            if(Server < 0){
                perror("Error While accepting");
                exit(1);
            }
            
            printf("bubu!\n");
            printf("desr = %d \n", Server);
            
            pthread_t ServerThread;
            int bubu = pthread_create(&ServerThread, NULL, ServerJob, &Server);
            if(bubu != 0)
                perror("Creating pthread error");
        }
        

        
        
        
    }
    
    else {
        
        
        
//        int PORT;
//        char* IP = malloc(255);
//        printf("Enter Port Number : ");
//        scanf("%d", &PORT);
//        printf("Enter IP Number : ");
//        scanf("%s", IP);

        
        
        
        
//        OurAddress.sin_family = AF_INET;
//        OurAddress.sin_port = htons(PORT);
//        OurAddress.sin_addr.s_addr = inet_addr(IP);
        if(connect(SocketFD, (struct sockaddr *)&OurAddress, sizeof(OurAddress)) < 0){
            perror("Client connection Error");
            exit(1);
        }
        
        
        struct DatMessage* NewMessage = (struct DatMessage*) malloc(sizeof(struct DatMessage*));
        for(int i = 0; i < HowManyMessages; ++i){
            NewMessage -> NameLen = 0;
            NewMessage -> DataSize = 0;
            NewMessage -> Data[0] = '\0';
            NewMessage -> Name[0] = '\0';
            char FileName[1 << 10];
            char Data[1 << 22];
            printf("Please, enter  Name of File: ");
            scanf("%s", FileName);
            
            char* FilePath = malloc(1 << 10);
            strcpy(FilePath, "/Users/semenfedotov/FilesForThreads/");
            strcat(FilePath, FileName);
            
            
            printf("FileName = %s\n", FilePath);
            
            int ClientDescr = open(FilePath, O_RDWR | O_CREAT | S_IRWXU, 0777);
            if(ClientDescr < 0) {
                perror("Error While opening Client File");
                exit(1);
            }
            
            struct stat OurStatistic;
            int Check = fstat(ClientDescr, &OurStatistic);
            if(Check < 0) {
                perror("Error while getting statistics");
                exit(1);
            }
            
            printf("Size = %d\n", OurStatistic.st_size);
            
            int NumBytes = 0;
            int Counter = 0;
            while (NumBytes < OurStatistic.st_size) {
               NumBytes += read(ClientDescr, NewMessage -> Data + NumBytes, 2);
               ++Counter;
                printf("Counter = %d\n", Counter);
                printf("NumBytes = %d\n", NumBytes);
            }
            if(NumBytes < 0) {
                perror("Error while reading Client File");
                exit(1);
            }
            NumBytes = 0;
            Counter = 0;
            close(ClientDescr);
            
            
            NewMessage -> NameLen = strlen(FileName);
            NewMessage -> DataSize = OurStatistic.st_size;
            memcpy(NewMessage -> Name, FileName, strlen(FileName));
            printf("Data = %s DataSize = %d\n", NewMessage -> Data, NewMessage -> DataSize);
            printf("ToSend Name = %s\n", NewMessage -> Name);
            
            ToSend(SocketFD, NewMessage, 2 * sizeof(int) + (1 << 10) + (1 << 22), 0);
        }
        
        free(NewMessage);

        
        
        
    }
    
    close(SocketFD);

    
    return 0;
}
