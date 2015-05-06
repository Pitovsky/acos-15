#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define usersLimit 16

char str[1<<10];
struct sockaddr_in socketAddress;
int fd, connected[usersLimit];

void sender(int socketFD, char *msg, size_t msgLength){
    int currentLength = 0;
    ssize_t msgPartLength;
    while(currentLength < msgLength){
        msgPartLength = send(socketFD, msg + currentLength, msgLength - currentLength, 0);
        if (msgPartLength == -1)
            break;
        currentLength += msgPartLength;
    }
}

static void *listenUser(void* id){
    int currentID = *((int*)id);
    while (1){
        // сначала отравляется длина сообщения,
        int recievedLength;
        if (recv(connected[currentID], (void*)&recievedLength, sizeof(int), 0) <= 0)
            break;
        int currentLength = 0;
        // затем оно само
        while(currentLength < recievedLength){
            ssize_t msgPartLength = recv(connected[currentID], str + currentLength, recievedLength - currentLength, 0);
            currentLength += msgPartLength;
        }
        str[recievedLength] = '\0';
        printf("user %d: %s\n", currentID, str);
        for (int i = 0; i < usersLimit; ++i)
            if (i != currentID && connected[currentID] != -1)
                sender(connected[i], str, recievedLength);
    }
    pthread_exit(NULL);
}

static void *getMsgFromServer(void* nothing){
    while(1){
        connect(fd, (struct sockaddr *)&socketAddress, sizeof(socketAddress));
        recv(fd, str, 512, 0);
        printf("server: %s\n", str);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    printf("client+port+ip or server+port\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    int port;
    sscanf(argv[2], "%d", &port);
    socketAddress.sin_port = htons(port);
    socketAddress.sin_family = AF_INET;
    if (strcmp(argv[1], "client") == 0){
        inet_aton(argv[3], &(socketAddress.sin_addr));
        pthread_t reciever;
        pthread_create(&reciever, NULL, getMsgFromServer, NULL);
        char* input = (char*)malloc(1<<10*sizeof(char));
        while (1){
            scanf("%s\n", input);
            if (strcmp(input, "exit") == 0)
                break;
            else{
                size_t msgLength = strlen(input);
                connect(fd, (struct sockaddr*)&socketAddress, sizeof(socketAddress));
                sender(fd, (void*)&msgLength, sizeof(int));
                sender(fd, input, msgLength);
            }
        }
        pthread_cancel(reciever);
        pthread_join(reciever, NULL);
    }else{
        socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(fd, (struct sockaddr*)&socketAddress, sizeof(socketAddress));
        listen(fd, 16);
        for (int i = 0; i < usersLimit; ++i)
            connected[i] = -1;
        while(1){
            int nextFD = accept(fd, NULL, NULL);
            int nextID = 0;
            while (nextID < usersLimit && connected[nextID] != -1)
                ++nextID;
            connected[nextID] = nextFD;
            pthread_t usrThread;
            pthread_create(&usrThread, NULL, listenUser, (void*)&nextID);
        }
    }
    close(fd);
    return 0;
}