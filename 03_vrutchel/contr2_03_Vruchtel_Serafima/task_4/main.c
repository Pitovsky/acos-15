/*компилировать с флагом -lpthread*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

// проверяет символ на допустимый
int isWordCharacter(char c) {
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9')) || (c == '_');
}

//пока не сделала, но нужно, чтобы каждый из потококв считал число строк в конкретном куске файла
//Функция, которая будет счить число слов в файле
void* findWordsInFile(void *arg, int fd) {//а передам ка я дескриптор
    int wCount = 0; // число слов в файле
    int lastChar = 0; // предыдущий символ - часть слова
    char buf;//буфер для символа

    //считываем посимвольно
    while(read(fd, &buf, 1) > 0) {
        if(!isWordCharacter(buf) && lastChar) {
            ++wCount;
        }
        lastChar = isWordCharacter(buf);
    }
    // если информация в файле кончилась, но последний символ принадлежал слову
    if(lastChar) {
        ++wCount;
    }

    // закрываем файл
    close(fd);

    // возвращаем результат
    printf("The count of words is: %d", wCount);
 //   return wCount;
}

int main(int argc, char **argv)//argv[1] - путь до файла, argv[2] - число потоков, которые необходимо использовать
{
    int fd = open(argv[1], O_RDWR);
    if(fd < 0) {
        perror("Problems with opening file\n");
        exit(-1);
    }
    //здесь определим размер открытого файла
    struct stat info_file;
    fstat(fd, &info_file);
    int size = info_file.st_size;
    //!!!Отладочный вывод!
    printf("%d\n", size);

    //считаем argv[2]
    int threads_count;//число потоков
    char *threads_count_read = (char*)malloc(256);
    strcpy(threads_count_read, argv[2]);
    threads_count = atoi(threads_count_read);
    printf("%d\n", threads_count);

  /*  char *buf = (char*)malloc(size);
    int num = read(fd, buf, size);
    if(num < 0) {
        perror("Problems with reading from file");
        exit(errno);
    }

    //Для отладки выведем содержимое buf на экран
    num = write(1, buf, size);
    if(num < 0) {
        perror("Problems with writing");
        exit(errno);
    }*/

    //а вообще лучше сделать отображение файла на память и отправлять никак не дескриптор
    char *pointer = (char*)malloc(8);
    void *file_addr = mmap((void*)pointer, //при удачном выполнении mmap возвращает указатель на область с отражёнными данными
                            size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,
                            0
                            );
    if(file_addr == NULL) {
        perror("Map failed\n");
        exit(errno);
    }

    //теперь разберёмся, что тут сделать с потокам
    pthread_t threads[threads_count];//создаём массив дескрипторов потоков
    int return_code;
    int i;
    for(i = 0; i < threads_count; i++) {
        return_code = pthread_create(&(threads[i]), 0, findWordsInFile, 0);
    }

    //потокам нужно разделять задания: size/threads_count (+последнему, возможно, задание нужно будет больше дать)
    int tasks_count = size/threads_count;

    for(i = 0; i < threads_count; i++) {
        close(threads[i]);
    }

    return 0;
}
