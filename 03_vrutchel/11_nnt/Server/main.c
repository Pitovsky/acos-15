/*Компилировать нужно с флагом -lpthread*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#define PORT 2306

int socketRead(void * arg)
{
    char buf[1024];
    int socket_ofclient = (arg);

    // считывание данных от клиента
    int readed = 0;
    int name_length, content_length;
    int filefd;

    // 1. получение длины имени файла
    readed = read(socket_ofclient, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The first read is wrong");
        exit(1);
    }
    name_length = *((int *) buf);
    printf("[%d] File length is %d\n", socket_ofclient, name_length);

    // 2. получаение имени файла
    readed = read(socket_ofclient, buf, name_length);
    if(readed != name_length) {
        perror("The second read is wrong");
        exit(1);
    }

    buf[name_length] = '\0';
    filefd = open(buf, O_WRONLY | O_CREAT, 0775);
    if(filefd < 0) {
        perror("Can't open file");
        exit(1);
    }
    printf("[%d] File name is %s\n", socket_ofclient, buf);

    // 3. получение длины содержимого файла
    readed = read(socket_ofclient, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The third read is wrong");
        exit(1);
    }
    content_length = *((int *) buf);
    printf("[%d] Content length is %d\n", socket_ofclient, content_length);

    // 4. получение содержимого файла
    readed = 0;
    int internal_readed = 0;
    do {
        internal_readed = read(socket_ofclient, buf, 1023);
        write(filefd, buf, internal_readed);

        buf[internal_readed] = '\0';
        printf("[%d] Read: %s\n", socket_ofclient, buf);

        readed += internal_readed;
    } while(readed < content_length);

    printf("[%d] End of writing file\n", socket_ofclient);

    //Всё, пора заканчивать
    close(filefd);
    close(socket_ofclient);

    printf("[%d] Closed all file descriptors\n", socket_ofclient);

    return 0;
}

int main(int argc, char** argv)
{
    pthread_t client_th;
    int socket_ofclient, listener;
    struct sockaddr_in addr;

    listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0) { //В случае ошибки возвращаемое значение равно -1
        perror("Problems with creating socket in server");
        exit(errno);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);//htons() преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl() преобразует узловой порядок расположения байтов положительного целого hostlong в сетевой порядок расположения байтов
    //Если при вызове bind указать INADDR_ANY, то сокет будет связан со всеми локальными интерфейсами

    if(bind(listener, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Problems with binding");
        exit(errno);
    }

    listen(listener, 1);

    printf("Listen at port %d!\n", PORT);

    while(1) {
        socket_ofclient = accept(listener, NULL, NULL);
        //эта функция извлекает первый запрос на соединение из очереди
        //создаёт новый поключенный сокет почти с такими же параметрами, как и у listener
        //выделяет для сокета новый файловый дескриптор, который и возвращается
        //новый сокет более не находится в слушающем состоянии
        //исходный сокет не изменяется при этом вызове
        if(socket_ofclient < 0) { //в случае ошибки возвращается -1
            perror("Can't make connection");
            exit(-1);
        }

        printf("New client accepted: %d!\n", socket_ofclient);

        //создание новой нити
        int result = pthread_create(&client_th,  (pthread_attr_t *)NULL, socketRead, socket_ofclient);
        if(result != 0)
        {
            printf ("Error on thread create, return value = %d\n", result);
            exit(-1);
        }
    }

    return 0;
}
