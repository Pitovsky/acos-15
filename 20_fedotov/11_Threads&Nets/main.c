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
#include <limits.h>

int SocketFD;
struct sockaddr_in OurAddress;

struct DatMessage{
    int NameLen;
    int DataSize;
    char Name[1 << 10];
    char Data[1 << 22];
};


long long ToSend(int SocketFileDesc, void* OurMessage, int MessageSize, int Flags) {
    long long Sent = 0;
    
    while (Sent < MessageSize) {
        int BytesNumber = send(SocketFileDesc, OurMessage + Sent, MessageSize - Sent, 0);
        if(BytesNumber < 0)
            return 0;
        
        Sent += BytesNumber;
    }
    
    return Sent;
}



long long ToRecieve(int SocketFileDesc, void* OurMessage, int MessageSize, int Flags) {
    long long Recieved = 0;
    
    while (Recieved < MessageSize) {
        int BytesNumber = recv(SocketFileDesc, OurMessage + Recieved, MessageSize - Recieved, 0);
        if(BytesNumber < 0)
            return 0;
        
        Recieved += BytesNumber;
    }
    
    return Recieved;
}





void* ServerJob(int* SocketFromDescr) {
    int Counter = 0;
    printf("---------Hello, Thread-----------\n");
    pthread_t SocketFD = *SocketFromDescr;
    
    while (1) {
        char* Recieved = malloc(40000000000);
        int NameLen = 0;
        long long DataLen = 0;
        char* FileName = malloc(1 << 10);
        memset(FileName, 0, 1 << 10);
        
        int Res = connect(SocketFD, (struct sockaddr*)&OurAddress, sizeof(OurAddress));
        
        ToRecieve(SocketFD, (void*)&NameLen, sizeof(int), 0);
        ToRecieve(SocketFD, (void*)&DataLen, sizeof(long long), 0);
        ToRecieve(SocketFD, FileName, NameLen, 0);
        ToRecieve(SocketFD, Recieved, DataLen, 0);
        
        
        if(NameLen == 0)
            continue;
        
        
        printf("_-'-_|_-'-_[Server've got the message!]_-'-_|_-'-_\n_!_!_INFO_!_!_\n");
        
        
        printf("- NameLen --> %d\n", NameLen);
        printf("- Name --> %s\n", FileName);
        printf("- DataSize --> %lld \n", DataLen);
        printf("- Data --> %s\n", Recieved);
        
        char* ToOpenFile = malloc(1 << 10);
        strcpy(ToOpenFile, "/Users/semenfedotov/Threads/");
        strcat(ToOpenFile, FileName);
        free(FileName);
        
        
        printf("FileNaame is = %s\n", ToOpenFile);
        
        int DatFileFD = open(ToOpenFile, O_RDWR | O_CREAT | S_IRWXU, 0777);
        if(DatFileFD < 0) {
            perror("Error while opening Server File");
            exit(1);
        }
        
        long long NumberWritten = 0;
        while (NumberWritten < DataLen) {
            NumberWritten += write(DatFileFD, Recieved + NumberWritten, 4096);
        }
        if(NumberWritten < 0) {
            perror("Error while Writing Data into File");
            exit(1);
        }
        
        close(DatFileFD);
        
        
        free(ToOpenFile);
        free(Recieved);
    }
    
    
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
        printf("/\-/\-/\-Hello, Today you're a Server-/\-/\-/\\n");
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
        printf("Enter IP : ");

//        printf("/\.'.'.'.'.You're A Client!.'.'.'.'.'/\\n");
//        printf("\/-_-_-____-_-_-____-_-_-____-_-_-___\/\n");
//        
//        int PORT;
//        char* IP = malloc(255);
//        printf("Enter Port Number : ");
//        scanf("%d", &PORT);
//        printf("Enter IP Number : ");
//        scanf("%s", IP);
        char * IP = malloc(255);
        scanf("%s", IP);

        OurAddress.sin_addr.s_addr = inet_addr(IP);

        
        
        
//        OurAddress.sin_family = AF_INET;
//        OurAddress.sin_port = htons(PORT);
//        OurAddress.sin_addr.s_addr = inet_addr(IP);
        if(connect(SocketFD, (struct sockaddr *)&OurAddress, sizeof(OurAddress)) < 0){
            perror("Client connection Error");
            exit(1);
        }
        
        
        for(int i = 0; i < HowManyMessages; ++i){
            char* FileName = malloc(1 << 10);
            char* Data = malloc(2000000000);
            int NameLen = 0;
            long long FileSize = 0;
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
            
            
            
            printf("Size = %lld\n", OurStatistic.st_size);
            
            long long NumBytes = 0;
            long  long Counter = 0;
            while (NumBytes < OurStatistic.st_size) {
               NumBytes += read(ClientDescr, Data + NumBytes, 4096);
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
            
            NameLen = strlen(FileName);
            FileSize = OurStatistic.st_size;
            
            
            printf("ToSend Name = %s\n", FilePath);
            
            
            printf("Data !!! = %s\n Size = %d", Data, strlen(Data));
            printf("FileSize = %lld\n", FileSize);
            
            ToSend(SocketFD, (void*)&NameLen, sizeof(int), 0);
            ToSend(SocketFD, (void*)&FileSize, sizeof(long long), 0);
            ToSend(SocketFD, FileName, NameLen, 0);
            ToSend(SocketFD, Data, FileSize, 0);
        
            free(FilePath);
            free(FileName);
            free(Data);
        }
        

        
        
        
    }
    
    close(SocketFD);

    
    return 0;
}
