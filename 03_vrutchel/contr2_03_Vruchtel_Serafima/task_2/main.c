/*При компиляции необходимо писать флаг -lm*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>

//тут будет функция считающая "буквенную энтропию"
double Counter(char *buf, int left, int right) {
    printf("I am in function\n");
    double res_sum = 0;//результирующая сумма - буквенная энтропия для данного диапазона
    int count;//переменная, в которой будем хранить, сколько раз встречается данный символ в диапазоне
    double p;//частота символа для каждой конкретной буквы

    //делаем цикл подсчёта результирующей суммы
    char i;
    int j;
    printf("I'm ready to go into cycle\n");
    for(i = 0; i < 256; i++) {
        printf("I'm into the first cycle\n");
        count = 0;
        //дальше идёт цикл прохода по buf
        for(j = left; j < right; j++) {
            printf("I'm into the second cycle\n");
            if((buf[j] & i) == 1) {
                printf("I'm into IF");
                printf("buf[%d] = %c", j, buf[j]);
                count++;
            }
        }
        p = count/(right - left);
        res_sum += ((-p) * log2(p));
    }

    return res_sum;
}

int main(int argc, char **argv)//argv[1] - путь до файла
{
    //тут откроем файл
    int fd = open(argv[1], O_RDWR);
    //проверим на возможные ошибки
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

    //чтобы было удобнее работать с файлом, отразим его на память
    char *pointer = NULL;
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

    //с mmap не получилось, попробую использовать просто буфер
    char *buf = (char*)malloc(size);
    int num = read(fd, buf, size);
    if(num < 0) {
        perror("Problems with reading from file");
        exit(errno);
    }

    int left;//левая граница, включается
    int right;//правая граница, включается

    //дальше будет работа с каждым из конкретных диапазонов
    while(1) {
        //попросим пользователя ввести диапазон, так будет легче
        printf("Please, enter the new range (if you want to finish enter -1 -1): ");
        scanf("%d %d", &left, &right);
        //!!!отладочный вывод
        printf("%d %d\n", left, right);
        if(left == -1) {
            printf("Finish work\n");
            break;
        }
        if(right < left) {
            printf("Invalid input\n");
            continue;
        }
        double res = Counter(file_addr, left, right);
        //printf("Result: %lf\n", res);
    }


    //в конце, как положено, нужно всё удалить
    close(fd);
    munmap(file_addr, size);
    return 0;
}
