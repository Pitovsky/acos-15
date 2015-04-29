#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>

//Аргументы: 1 - режим работы (клиент или сервер), 2 - port, 3 - ip(только для клиента, ip сервера будем считать равным 1)
int main(int argc, char ** argv)
{
    if(argc < 3) {//всё плохо, сваливаем отсюда
        printf("To few arguments\n");
        exit(-1);
    }

    if(strcmp(argv[1], "server") == 0) {/*работает сервер*/
        printf("I'm server\n");

        //Считываем оставшиеся аргументы командной строки
        char *port = (char*)malloc(256);
        strcpy(port, argv[2]);

//отладночный вывод
            printf("%s\n", port);

        int listener;//будущий дескриптор сокета
        struct sockaddr_in addr;//Структура-описатель сокета
        //!!!может, это неправильно, но тогда я не знаю, как реализовать нормально пункт "сообщения не должны рваться"
        char buf[2048];

        listener = socket(AF_INET, SOCK_STREAM, 0);//создаём сокет для входных подключений
        if(listener < 0) {
            perror("Failed the creation of socket");
            exit(errno);
        }

        //Заполняем структуру addr
        addr.sin_family = AF_INET;
        addr.sin_port = htons((uint16_t)*port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        //Связывание сокета
        if(bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("Errors with binding");
            exit(errno);
        }

        listen(listener, 2);

        fd_set readf;//переменная для select
        fd_set writef;//переменная для select
        struct timeval time;//тайм-аут для select
        int select;//переменная для возвращаемого значения select
        int nlistener;//дескриптор, полученный с помощью accept

        while(1) {
            FD_ZERO(&readf);
            FD_ZERO(&writef);
            FD_SET(listener, &readf);
            FD_SET(listener, &writef);
            time.tv_sec = 10;

            select = select(listener+1, &readf, &writef, 0, &time);

            if(select == 0) {
                perror("timeout\n");
                continue;
            }

            if((FD_ISSET(listener, &readf)) || if((FD_ISSET(listener, &writef))){
                nlistener = accept(listener, NULL, NULL);
                if(nlistener < 0){//в случае ошибки возвращается -1
                    perror("Can't make connection\n");
                    exit(errno);
                }
                //дальше будет пересылка сообщений
            }


         }

    } else if(strcmp(argv[1], "client") == 0) {/*работает клиент*/
        printf("I'm client\n");

        //Считываем оставшиеся аргументы командной строки
        char *port = (char*)malloc(256);
        char *ip = (char*)malloc(256);
        strcpy(port, argv[2]);
        strcpy(ip, argv[3]);
//отладочный вывод
                printf("%s %s\n", port, ip);

        int sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
            perror("Can't create sock");
            return (1);
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons((uint16_t)*port);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//тут пока точно не знаю, что должо быть

        if(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0){//присваивает сокету удалённый адрес
            perror("Can't connect");//если произошла ошибка, то возвращается -1
            return (1);

        //write(sock, "abc\n",4);
        //char buf[1024];
        //int got = read(sock, buf, 1024);

        //write(1, buf, got);

        }

    }

    return 0;
}
