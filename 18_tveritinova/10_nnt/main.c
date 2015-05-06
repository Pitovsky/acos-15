#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>

void* thread_func(void* argv)
{
    //printf("in thread %d, get socket %d\n", ((int*) argv)[1], ((int*) argv)[0]);
    int fd = ((int*) argv)[0];
    int thread_number = ((int*) argv)[0];
    int counter_while = 0;
    while(1)
    {
        int name_length;
        int er_read = read(fd, &name_length, sizeof(int));
        //printf("get name_length = %d\n", name_length);
        if (er_read == 0)
        {
            printf("client exit\n");
            break;
        }
        char name[name_length];
        for (int i = 0; i <= name_length; i++)
        {
            read(fd, &name[i], sizeof(char));
        }
        //printf("get name: %s\n", name);
        int creat_file = creat(name, 00700);
        if (fd == -1)
        {
            perror("creat");
        }

        int mess_length;
        read(fd, &mess_length, sizeof(int));
        //printf("get mess_length = %d\n", mess_length);
        char mess[mess_length];
        for (int i = 0; i <= mess_length; i++)
        {
            read(fd, &mess[i], sizeof(char));
        }
        //printf("get mess: %s\n", mess);
        write(creat_file, mess, sizeof(char) * mess_length);
        close(creat_file);
        printf("create file: %s\n", name);
        ++counter_while;
    }
}


void process_with_server(int port)
{
    printf("server started\n");
    struct sockaddr_in serv_addr;
    int listener;
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
    }
    int counter_thread = 0;
    int argv[2];
    while(1)
    {
        if (listen(listener, 1) == -1)
        {
            perror("listen");
        }
        int client;
        if ((client = accept(listener, NULL, NULL)) < 0)
        {   
            perror("accept");
        }
        pthread_t thread;
        argv[0] = client;
        argv[1] = counter_thread;
        ++counter_thread;
        if (pthread_create(&thread, NULL, thread_func, argv) < 0)
        {
            perror("pthread_creat");
        }
    }
}

void process_with_client(int port, char* ip)
{
    printf("HELLO DEAR USER!\nYou can send to server your message and server will save it in file\nJust enter in first string name of file and in second string - text\nto exit write \"exit\"\nGOOD LUCK!\n");
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

    char name[1024];
    char mess[1024];
    int mess_size, name_size;
    char send_mess[1024];
    int counter_send_mess = 0;
    char stream;
    int counter_mess, counter_name;
    while(1)
    {
       // scanf("%s", name);
        //getline(&name, &name_size, stdin);
        counter_name = 0;
        printf("string first(for name) - PLEASE ENTER NAME OF FILE: ");
        scanf("%c",&stream);
        while (stream != '\n')
        {
            name[counter_name] = stream;
            ++counter_name;
            scanf("%c", &stream);
        }
        name[counter_name] = 0;
        //printf("get name: %s\n", name);
        if (strcmp(name, "exit") == 0)
        {
            close(sock);
            break;
        }
        name_size = strlen(name);
        ((int*) &(send_mess[counter_send_mess]))[0] = name_size;
        counter_send_mess += sizeof(int);
        sprintf(&send_mess[counter_send_mess], "%s", name);
        counter_send_mess += name_size + 1;
        
        //scanf("%s", mess);
        //getline(&mess, &mess_size, stdin);  
        //gets(mess);
        // ни одна из функций не сработала корректно, поэтому буду считывать посимвольно
        counter_mess = 0;
        printf("string second(for text) - PLEASE ENTER TEXT OF FILE: ");
        scanf("%c",&stream);
        while (stream != '\n')
        {
            mess[counter_mess] = stream;
            ++counter_mess;
            scanf("%c", &stream);
        }
        mess[counter_mess] = 0;
        //printf("get mess: %s\n", mess);
        mess_size = strlen(mess);
        ((int*) &(send_mess[counter_send_mess]))[0] = mess_size;
        counter_send_mess += sizeof(int);
        
        sprintf(&send_mess[counter_send_mess], "%s", mess);
        counter_send_mess += mess_size + 1;
        write(sock, send_mess, counter_send_mess);
        for (int i = 0; i < mess_size + name_size + 2 + 2 * sizeof(int); i++)
        {
            send_mess[i] = 0;
        }
        counter_send_mess = 0;
    }
}


int main(int argc, char** argv)
{
    char mode = argv[1][0]; // 0 - сервер, 1 - клиент
    if (mode == '0')
    {
        process_with_server(atoi(argv[2]));
    }
    if (mode == '1')
    {
        process_with_client(atoi(argv[2]), argv[3]);
    }
}
