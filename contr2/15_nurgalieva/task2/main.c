#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv)
{
    char* path = argv[1];
    int fd = open(argv[1],
                    O_RDWR|O_CREAT,
                    S_IRWXU);

    struct stat status;
    stat(path, &status);
    int j;
    double sum = 0;

    char * file_pointer = mmap(0, status.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    int* count = (int*)calloc(256, sizeof(int));
    int beg, end;

    while (1)
    {
        if (scanf("%d%d", &beg, &end) < 0)
        {
            break;
        }
        if ((beg <= end) && (end < status.st_size))
        {
            for (j = beg; j < end; ++j)
            {
                ++count[*(file_pointer + j)];//надо приводить к беззнаковому типу
            }
            for (j = 0; j < 255; ++j)
            {
                if (count[j] != 0)
                {
                    sum += ((count[j] * log2(count[j])) / (end - beg));
                }
        }
        printf("Answer: %lf\n", sum);
        }
    }

    return 0;
}
