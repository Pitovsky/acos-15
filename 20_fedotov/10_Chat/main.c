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

int HowManyMessages = 10;





int main(int argc, const char * argv[]) {
    setbuf(stdout, 0);
    
    if(strcmp(argv[1], "Server") == 0) {//Server
        int PORT;
        printf("Please, enter Port Number : ");
        scanf("%d", &PORT);
        
        char* ServerBuff = malloc(1 << 12);
        
        struct sockaddr_in OurAddress;
        char Buffer[1024];
        int BytesNumber;
        int Server;
        
        
        int Listener = socket(AF_INET, SOCK_STREAM, 0);
        if(Listener < 0) {
            perror("Error While socketing Client");
            exit(1);
        }
        
        
        OurAddress.sin_family = AF_INET;
        OurAddress.sin_port = htons(PORT);
        OurAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        
        if(bind(Listener, (struct sockaddr*)&OurAddress, sizeof(OurAddress)) < 0) {
            perror("Binding Error");
            exit(1);
        }
        
        listen(Listener, HowManyMessages);
        
        int Descriptors[Listener + 1];
        memset(Descriptors, 0, (Listener + 1) * sizeof(int));
        
        int counter = 0;

        while (counter < HowManyMessages) {
            fd_set ReadSet;
            FD_ZERO(&ReadSet);
            FD_SET(Listener, &ReadSet);
            int Maximal = Listener;
            for(int i = 0; i <= Listener; ++i) {
                if(Descriptors[i] != 0)
                    FD_SET(Descriptors[i], &ReadSet);
                if(Descriptors[i] > Maximal)
                    Maximal = Descriptors[i];
            }
            
            int ReadyNumber = select(Maximal + 1, &ReadSet, 0, 0, 0);
            if(ReadyNumber < 0)
                perror("Error while select descriptors");
            
            if(FD_ISSET(Listener, &ReadSet)) {
                Server = accept(Listener,NULL, NULL);
                if(Server < 0){
                    perror("Error While accepting");
                    exit(1);
                }
                
                int i;
                for(i = 0; i <= Listener; ++i){
                    if(Descriptors[i] == 0) {
                        Descriptors[i] = Server;
                        i = -1;
                        break;
                    }
                }
                if(i > 0) {
                    perror("Too many connections");
                    exit(1);
                }
                
            }
            for(int i = 0; i <= Listener; ++i){
                if(Descriptors[i] != 0 && FD_ISSET(Descriptors[i], &ReadSet)){
                    BytesNumber = recv(Descriptors[i], Buffer, 1024, 0);
                    if(BytesNumber <= 0) {
                        close(Descriptors[i]);
                        Descriptors[i] = 0;
                        continue;
                    }
                    printf("______Server've got the message!_______\n");
                    strcat(Buffer, "\n");
                    strcat(ServerBuff, Buffer);
                    ++counter;
                    int bubu = send(Server, Buffer, BytesNumber + 1, 0);
                    if(bubu < 0) {
                        perror("Error while Sending message");
                        exit(1);
                    }
                }
            }
        
            
        }
        
        
        printf("All from server: \n %s", ServerBuff);
        
        
        
    }

    else {
        
        printf("How many messages would you like to send???: ");
        scanf("%d", &HowManyMessages);
        
        int PORT;
        char* IP = malloc(255);
        printf("Enter Port Number : ");
        scanf("%d", &PORT);
        printf("Enter IP Number : ");
        scanf("%s", IP);
        

        
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
        
        char Buffer[1 << 12];
        for(int i = 0; i < HowManyMessages; ++i){
            fgets(Buffer, (1 << 12) , stdin);
            send(Client, Buffer, strlen(Buffer) + 1, 0);
        }
        
        recv(Client, Buffer, (1 << 12), 0);
        
        close(Client);

    }
    
    return 0;
}
