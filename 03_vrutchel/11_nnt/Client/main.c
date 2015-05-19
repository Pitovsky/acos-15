/*аргументом argv[1] программе должен передаваться адрес сервера
* аргументом argv[2] программе должен передаваться путь до файла
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 2306

// возвращает длину имени файла по его пути
int fileNameLengthByPath(const char * filePath) {
    if(filePath == NULL)
        return 0;

    if(strrchr(filePath, '/') == NULL)
        return strlen(filePath);

    int lastSlashPosition = (int)(strrchr(filePath, '/') - filePath);
    return strlen(filePath) - lastSlashPosition - 1;
}

int main(int argc, char **argv)
{
    //если аргумент не получен, то, видимо, сваливаем (ну или ещё как вариант можно попросить ввести его с клавиатуры, но это потом)
    if(argc < 2) {
        printf("No server ip\n");
        exit(-1);
    }

     if(argc < 3) {//В argv[2] будем хранить адрес файла, который надо открыть, и содержимое которого надо отправить на сервер
        printf("No file address\n");
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

    //сообщение у нас будет создаваться: открываем файл и формируем сообщение
    int fd = open(argv[2], O_RDONLY);
    if(fd == -1) {
        perror("Problems with opening file");
        exit(errno);
    }

    //здесь определим размер открытого файла
    struct stat info_file;
    fstat(fd, &info_file);
    int info_length = info_file.st_size;//длина содержимого

    //получаем длину имени файла
    int name_length = fileNameLengthByPath(argv[2]);

    //получаем имя файла
    char name[name_length];
    memcpy(name, argv[2] + strlen(argv[2]) - name_length, name_length);

    //получаем содержимое файла
    char info[info_length];
    int res = read(fd, info, info_length);
    if(res < 0) {
        perror("Problems with reading from file");
        exit(errno);
    }

    //потом отправляем сообщение серверу
    res = send(sock, (char*)(&name_length), sizeof(int), 0);
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

    close(fd);
    close(sock);
    return 0;
}
