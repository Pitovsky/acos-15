/*аргументом argv[1] программе должен передаваться адрес сервера*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2306

int main(int argc, char **argv)
{
    //если аргумент не получен, то, видимо, сваливаем (ну или ещё как вариант можно попросить ввести его с клавиатуры, но это потом)
    if(argc < 2) {
        printf("No server ip\n");
        exit(-1);
    }

    //если argv[1] существует, то его надо получить
    //char *server_ip = (char*)malloc(16);//xxx.yyy.zzz.uuu\0
    //strcpy(server_ip, argv[1]);

    //тут нужно создать сокет
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Problems with creating socket in client");
        exit(errno);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);//тут не уверена по поводу используемой функции
    addr.sin_addr.s_addr = inet_addr(argv[1]);//и здесь естественно тоже

    //поехали коннектиться с сервером
    if(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0){//иницирует соединение на сокете
        perror("Problems in connecting with server");//если произошла ошибка, то возвращается -1
        exit(errno);
    }

    //теперь создаём сообщение, которое надо будет переслать
    int name_length, info_length;
    printf("Enter the length of file-name: ");
    scanf("%d", &name_length);
    char name[name_length];
    printf("Enter the name of file: ");
    scanf("%s", name);
    printf("Enter the length of information: ");
    scanf("%d", &info_length);
    char info[info_length];
    printf("Enter the information: ");
    scanf("%s", info);

    //потом отправляем сообщение серверу
    int res = send(sock, (char*)(&name_length), sizeof(int), 0);
    if(res == -1) {
        perror("Problems with sending the length of file-name");
        exit(errno);
    }
    res = send(sock, name, name_length, 0);
    if(res == -1) {
        perror("Problems with sending the name of file");
        exit(errno);
    }
    res = send(sock, (char*)(&info_length), sizeof(int), 0);
    if(res == -1) {
        perror("Problems with sending the length of information");
        exit(errno);
    }
    res = send(sock, info, info_length, 0);
    if(res == -1) {
        perror("Problems with sending the information");
        exit(errno);
    }

    close(sock);
    return 0;
}
