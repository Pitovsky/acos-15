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
#include <time.h>


int SocketFD;
struct sockaddr_in OurAddress;
int UsersSockets[30];
int UsersNumber = 30;

struct Message{
    int FromWhoID;
    char* Text;
};


long long ToSend(int SocketFileDesc, void* OurMessage, long long MessageSize, int Flags) {
    long long Sent = 0;
    
    while (Sent < MessageSize) {
        int BytesNumber = send(SocketFileDesc, OurMessage + Sent, MessageSize - Sent, 0);
        if(BytesNumber < 0)
            return 0;
        
        Sent += BytesNumber;
    }
    
    return Sent;
}



long long ToRecieve(int SocketFileDesc, void* OurMessage, long long MessageSize, int Flags) {
    long long Recieved = 0;
    
    while (Recieved < MessageSize) {
        int BytesNumber = recv(SocketFileDesc, OurMessage + Recieved, MessageSize - Recieved, 0);
        if(BytesNumber < 0)
            return 0;
        
        Recieved += BytesNumber;
    }
    
    return Recieved;
}





void* ServerJob(int* UserID) {
    int Counter = 0;
    printf("---------Connected New User with _[%d]_ ID-----------\n", *UserID);
   
    int OurUserID = *UserID;
    while (1) {
        int KindOfMessage;
        long long MessageLength;
        char* Recieved = malloc(1 << 10);
        
        
        
        int IsHere = recv(UsersSockets[OurUserID], (void*)&KindOfMessage, sizeof(int), 0);
        if(IsHere <= 0) {
            printf("%d User has disconnected!\n", OurUserID);
            UsersSockets[OurUserID] = -1;
            break;
        }
        
        
        
        ToRecieve(UsersSockets[OurUserID], (void*)&MessageLength, sizeof(long long), 0);
        ToRecieve(UsersSockets[OurUserID], Recieved, MessageLength, 0);
        
        if(MessageLength == 0)
            continue;
        
        
//        printf("[Server've got the message!]\n");
        
        time_t DatTime;
        struct tm * TimeInfo;
        
        time ( &DatTime );
        TimeInfo = localtime ( &DatTime );
        char* OurTime = malloc(255);
        strcpy(OurTime, asctime(TimeInfo));
        OurTime[strlen(OurTime) - 1] = '\0';
        
        printf("[%s] | [%d]User -> %s\n", OurTime, OurUserID, Recieved);
        memset(OurTime, '\0', strlen(OurTime));
        free(OurTime);
        
        if(KindOfMessage == -1) {
            for(int i = 0; i < UsersNumber; ++i) {
                if(UsersSockets[i] != -1 && OurUserID != i) {
                    ToSend(UsersSockets[i], (void*)&MessageLength, sizeof(long long), 0);
                    ToSend(UsersSockets[i], Recieved, MessageLength, 0);
                    ToSend(UsersSockets[i], (void*)&OurUserID, sizeof(int), 0);
                }
            }
        }
        
        else {
            ToSend(UsersSockets[KindOfMessage], (void*)&MessageLength, sizeof(long long), 0);
            ToSend(UsersSockets[KindOfMessage], Recieved, MessageLength, 0);
            ToSend(UsersSockets[KindOfMessage], (void*)&OurUserID, sizeof(int), 0);
        }
        
        memset(Recieved, '\0', strlen(Recieved));
        free(Recieved);
    }
    
    
    pthread_exit(NULL);
    
    
}


void* ClientRecieving(void* Smth) {
    printf("--------Hello, Client Thread--------\n");
    
    while(1) {
        int Res = connect(SocketFD, (struct sockaddr*)&OurAddress, sizeof(OurAddress));
        long long MessageLength;
        int FromWho;
        char* Message = malloc(1 << 10);
        
        long long ErrOrNotFirst = ToRecieve(SocketFD, (void*)&MessageLength, sizeof(long long), 0);
        long long ErrOrNotSecond = ToRecieve(SocketFD, Message, MessageLength, 0);
        
        long long ErrOrNotThird = ToRecieve(SocketFD, (void*)&FromWho, sizeof(int), 0);
        
        if(ErrOrNotFirst <= 0 && ErrOrNotSecond <= 0 && ErrOrNotThird)
            continue;
        
        printf("[%d]User -->> %s\n", FromWho, Message);
        
        
        memset(Message, '\0', strlen(Message));
        free(Message);
    }
    pthread_exit(0);
}




int main(int argc, const char * argv[]) {
    setbuf(stdout, 0);
    char* Mode = malloc(10);
    scanf("%s", Mode);
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
    
    if(!strcmp(Mode, "Server")) {//Server
        printf("/\-/\-/\-Hello, Today you're a Server-/\-/\-/\ \n");
        OurAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        
        if(bind(SocketFD, (struct sockaddr*)&OurAddress, sizeof(OurAddress)) < 0) {
            perror("Binding Error");
            exit(1);
        }
        listen(SocketFD, UsersNumber);
        memset(UsersSockets, -1, UsersNumber * sizeof(int));
        
        printf("_-_-_-__-_-_-_Server Has started without errors_-_-_-__-_-_-_\n");
        
        while (1) {
            int ClientSocket = accept(SocketFD,NULL, NULL);
            if(ClientSocket < 0){
                perror("Error While accepting");
                exit(1);
            }
            
            
            int UserID = 0;
            while (UsersSockets[UserID] != -1 && UserID < UsersNumber) {
                ++UserID;
            }
            if(UserID == UsersNumber) {
                printf("Error! Too many Users online\n");
                continue;
            }
            UsersSockets[UserID] = ClientSocket;
            
            pthread_t ServerThread;
            int bubu = pthread_create(&ServerThread, NULL, ServerJob, &UserID);
            if(bubu != 0)
                perror("Creating pthread error in Server");
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
//       
        pthread_t ToRecieveClientThread;
        int bubu = pthread_create(&ToRecieveClientThread, NULL, ClientRecieving, NULL);
        if(bubu != 0)
            perror("Creating pthread error in Client");
//
        printf("Hi, now we're starting a Chat. Just enter your message and enjoy....\n");
        while(1){
            char* Message = malloc(1 << 10);
            long long MessageLength = 0;
            char WrittenSymbol = 0;
            
            int KindOfMessage;
            char Command[3];
            scanf("%s\n", Command);
            if(!strcmp(Command, "all"))
                KindOfMessage = -1;
            else
                KindOfMessage = atoi(Command);
            
            int PositionInMessage = 0;
            do {
                WrittenSymbol = getchar();
                Message[PositionInMessage] = WrittenSymbol;
                ++PositionInMessage;
            }while(WrittenSymbol != '\n' && WrittenSymbol != EOF);
            
            
            Message[strlen(Message) - 1] = '\0';
            MessageLength = strlen(Message);
            

            
            if(connect(SocketFD, (struct sockaddr *)&OurAddress, sizeof(OurAddress)) < 0){
//                perror("Client connection Error");
            }
            
            ToSend(SocketFD, (void*)&KindOfMessage, sizeof(int), 0);
            ToSend(SocketFD, (void*)&MessageLength, sizeof(long long), 0);
            ToSend(SocketFD, Message, MessageLength, 0);
            
            
            memset(Message, '\0', strlen(Message));
            free(Message);
        }
        
    }
    
    close(SocketFD);
    
    
    return 0;
}