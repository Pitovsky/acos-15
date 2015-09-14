#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char* argv[])
{
    struct stat fileinfo;
    int fin = open(argv[1], O_RDWR);
    fstat(fin, &fileinfo);

    int columns = atoi(argv[2]);
    int elements = fileinfo.st_size / sizeof(double);
    int rows = elements / columns + (elements % columns != 0);

    void* begin = mmap(0, fileinfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fin, 0);
    double* ptr = (double*)begin;

    char command[10];
    while (strcmp(command, "exit") != 0)
    {
        scanf("%s", command);
        if (strcmp(command, "getinfo") == 0)
        {
            printf("%d\n%d", columns, rows);
            continue;
        }
        if (strcmp(command, "get") == 0)
        {
            int x,y;
            scanf("%d%d", &x, &y);
            printf("%lf\n", *(ptr + x * columns + y));
            continue;
        }
        if (strcmp(command, "set") == 0)
        {
            int x,y;
            double value;
            scanf("%d%d", &x, &y);
            scanf("%lf", &value);
            *(ptr + x * columns + y) = value;
            continue;
        }
        if (strcmp(command, "sum") == 0)
        {
            int n;
            scanf("%s", &command);
            scanf("%d", &n);
            if (strcmp(command, "row") == 0)
            {
                double sum = 0;
                double* ptr_;
                for (ptr_ = ptr + n * columns; ptr_ != ptr + (n + 1) * columns; ptr_++)
                    sum += *(ptr_);
                printf("%lf\n", sum);
                continue;
            }
            if (strcmp(command, "col") == 0)
            {
                double sum = 0;
                double* ptr_;
                for (ptr_ = ptr + n; ptr_ <= ptr + (rows - 1) * columns + n; ptr_ += columns)
                    sum += *(ptr_);
                printf("%lf\n", sum);
                continue;
            }
            continue;
        }
        if (strcmp(command, "swap") == 0)
        {
            int x,y;
            scanf("%d%d", &x, &y);
            double buf;

            double* ptr1;
            double* ptr2;

            for (ptr1 = x * columns, ptr2 = y * columns; ptr1 != (x + 1) * columns, ptr2 != (y + 1) * columns; ptr1++, ptr2++)
            {
                buf = *ptr1;
                *ptr1 = *ptr2;
                *ptr2 = buf;
            }
        }
    }
}
