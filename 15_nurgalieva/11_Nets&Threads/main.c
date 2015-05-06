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

#define MAX_NUM_OF_CLIENTS 5
#define MAX_SIZE_OF_MESSAGE 200
#define PORT_NUM 9333

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int clients = 0;

int socketnum[MAX_NUM_OF_CLIENTS];
int connected[MAX_NUM_OF_CLIENTS];

typedef int
#define true 1
#define false 0
bool;

struct msg
{
    char* file_name;
    char* message;
};

struct msg* Msg;

void* thread_func(void * arg)
{
    int num = (int)(arg);
    int fd;
    pthread_mutex_lock(&mtx);
    int sock_id = socketnum[num];
    char buf[MAX_SIZE_OF_MESSAGE + 30];
    //printf("Msg[num].filename : %s\n", Msg[num].file_name);
    if (fd = open(Msg[num].file_name, O_RDONLY) == -1)
    {
        write(sock_id, "Problem with file\n", strlen("Problem with open file\n"));
    }
    else
    {
        sprintf(buf, "Server create file_name: %s with message: %s from clients: %d\n", Msg[num].file_name, Msg[num].message, num);
        write(sock_id, buf,sizeof(buf));
    }
    ++clients;
    close(sock_id);
    close(fd);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void msg_parser(char* msg, int ind)
{
    char* str = (char*)malloc(MAX_SIZE_OF_MESSAGE);
    str = "";
    Msg[ind].file_name = "";
    Msg[ind].message = "";
    int i = 0;
    int count = 0;

    while (count < 2)
    {
        while (msg[i] != ' ')
        {
            str += msg[i];
            ++i;
        }
        int size_of_name;
        size_of_name = atoi(str);
        (count == 0) ? (Msg[ind].file_name =(char*)malloc(size_of_name)) : (Msg[ind].message =(char*)malloc(size_of_name));
        ++i;
        while ((msg[i] != ' ')&&( msg[i] != '\0'))
        {
            (count == 0) ? (Msg[ind].file_name += msg[i]) :  (Msg[ind].message += msg[i]);
            ++i;
        }
        str = "";
        ++count;
    }
    //printf("file_name: %s with message: %s\n", Msg[ind].file_name, Msg[ind].message);

    int fd;
    if (fd = open(Msg[ind].file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU) == -1)
    {
        perror("problem with file\n");
    }
    write(fd, Msg[ind].message, strlen(Msg[ind].message));
    close(fd);
}

int main(int argc, char** argv)
{
    if (argc == 1) //значит, это сервер
    {
        int socket_id = 0;
        struct sockaddr_in addr;
        socket_id = socket(AF_INET, SOCK_STREAM, 0 );
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT_NUM);

        Msg = (struct msg*)malloc(MAX_NUM_OF_CLIENTS * sizeof(struct msg));
        struct sockaddr_in new_addr;
        pthread_t* thread = (pthread_t *)malloc(MAX_NUM_OF_CLIENTS*sizeof(pthread_t));
        int new_id;
        int new_size = sizeof(struct sockaddr_in);

        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(socket_id, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            perror("bind");
            exit(1);
        }

        listen(socket_id, MAX_NUM_OF_CLIENTS);

        while (true)
        {
            new_id = accept(socket_id, (struct sockaddr*)&new_addr, &new_size);
            if (new_id != -1)
            {
                 if (clients >= MAX_NUM_OF_CLIENTS)
                 {
                     write(new_id, "Too many clients\n", sizeof(char)*strlen("Too many clients\n"));
                     close(new_id);
                 }
                 else
                 {
                     pthread_mutex_lock(&mtx); // trylock!!!
                     int free_thread = 0;
                     while (connected[free_thread])
                     {
                         ++free_thread;
                     }
                    connected[free_thread] = 1;
                    socketnum[free_thread] = new_id;
                    char buf[MAX_SIZE_OF_MESSAGE];
                    int check;
                    if ((check = recv(new_id, buf, sizeof(buf), 0)) <= 0)
                    {
                        write(new_id, "Your message isn't recieve\n", sizeof(char)*strlen("Your message isn't recieve\n"));
                        close(new_id);
                        continue;
                    }
                    else
                    {
                        msg_parser(buf, free_thread);
                    }
                    pthread_create(&thread[free_thread], NULL, thread_func, (void*)free_thread);
                    pthread_detach(thread[free_thread]);
                    pthread_mutex_unlock(&mtx);
                 }
            }
        }
        close(socket_id);
    }
    else //клиент должен ввести имя хоста
    {
        int socket_id = 0;
        struct sockaddr_in addr;
        struct hostent* host;
        host = gethostbyname(argv[1]);

        socket_id = socket(AF_INET, SOCK_STREAM, 0 );
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT_NUM);
        memcpy(&addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
        while (connect(socket_id, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            if (errno == ENOENT)
            {
                printf("Please wait\n");
                sleep(1);
                continue;
            }
            else
            {
                perror("problem with connect\n");
                exit(1);
            }
        }
        printf("Connected with server\n");
        char buf[MAX_SIZE_OF_MESSAGE];
        fgets(buf,sizeof(buf),stdin);
        write(socket_id, buf, sizeof(buf));
        read(socket_id, buf, sizeof(buf));
        printf("Server send: %s \n", buf);
    }

    return 0;
}
