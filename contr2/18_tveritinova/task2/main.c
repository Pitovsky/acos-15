
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open");
    }

    struct stat file_stat;
    fstat(fd, &file_stat);
    
    char* file_addr;
    if ((file_addr = mmap(NULL, file_stat.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) 
    {
        perror("mmap");
    }
    int start, end;
    int er_read;
    char char_count[255];
    double res = 0;
    while(1)
    {
        er_read = scanf("%d %d", &start, &end);
        if (er_read == 0)//не верное сравнение. а елси как раз файл закончится?
        {
            break;
        }
        for (int i = start; i <= end; i++)
        {
            char_count[(int) file_addr[i]] = char_count[file_addr[i]] + 1;//надо к беззнаковому приводить
        }
        res = 0;
        for (int i = 0; i < 256; i++)
        {
            if (char_count[i] != 0)
            {
                res += (((double) char_count[i]) / (end - start)) * log(((double) char_count[i]) / (end - start)) / log(2);
            }
        }
        printf("=%lf\n", res);
    }
    munmap(file_addr, file_stat.st_size);
}
