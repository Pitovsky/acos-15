#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <assert.h>

typedef struct Matrix
{
    int col;
    int row;
    int flag;
    int size;
    int fd;
    double* shared_mem_ptr;
};

void Open(char* FName, Matrix* matrix)
{
    matrix->fd = open(FName, O_RDWR);

    if (matrix->fd == -1)
    {
         matrix->fd = open(FName,O_RDWR | O_CREAT, S_IRWXU);
         if (matrix->fd == -1)
         {
             perror("Can't open\n");
             _exit(EXIT_FAILURE);
         }
         matrix->flag = 1;
    }
    if(matrix->row == -1)
    {
        if(matrix->flag == 1)
        {
            fprintf(stderr, "Can't get size.\n");
            _exit(EXIT_FAILURE);;
        }
        else
        {
            struct stat info;
            matrix->size = info.st_size;
            matrix->row = matrix->size / (sizeof(double) * matrix->col);
        }
    }
    else
    {
        matrix->size = matrix->col * matrix->row * sizeof(double);
        ftruncate(matrix->fd, matrix->size);
        printf("New size is: %d\n", matrix->size);
    }
    matrix->shared_mem_ptr = mmap(NULL, matrix->size, PROT_READ | PROT_WRITE, MAP_SHARED, matrix->fd, 0);
    if (matrix->shared_mem_ptr == MAP_FAILED)
    {
        perror("mmap error\n");
    }
    close(matrix->fd);
}

void Input(int argc, char** argv, char** FName,
               int* col, int* row)
{
    if (argc < 3)
    {
        fprintf(stderr, "Wrong input.\n");
        _exit(EXIT_FAILURE);
    }
    *FName = argv[1];
    char* str_err;
    *col = strtol(argv[2], &str_err, 10);
    if (strcmp(str_err, "") != 0)
    {
        fprintf(stderr, "Wrong num of col.\n");
        _exit(EXIT_FAILURE);
    }
    *row = -1;
    if (argc == 4)
    {
        char* str_err;
        *row = strtol(argv[3], &str_err, 10);
        if (strcmp(str_err, "") != 0)
        {
            fprintf(stderr, "Wrong num of row.\n");
            _exit(EXIT_FAILURE);
        }
    }
}


int GetIndex(int x, int y, Matrix matrix)
{
    if (x < 0 || y < 0 || x >= matrix.row || y >= matrix.col)
        return -1;
    return x * matrix.col + y;
}

void Get(Matrix* matrix)
{
    int x, y;
    int ind = GetIndex(x, y, *matrix);
    printf("matrix[%d][%d] == %lf\n", x, y, matrix->shared_mem_ptr[ind]);
}

void Sum(Matrix* matrix)
{
    int i;
    char mode[10];
    if (strcmp(mode, "col") == 0)
    {
        double sum = 0;
        for (int k = 0; k < matrix->row; ++k)
        {
            int ind = GetIndex(k, i, *matrix);
            assert(ind >= 0);
            sum += matrix->shared_mem_ptr[ind];
        }

        printf("Col %d: sum == %lf\n", i, sum);
        return;
    }
    if (strcmp(mode, "row") == 0)
    {
        double sum = 0;
        for (int k = 0; k < matrix->col; ++k)
        {
            int ind = GetIndex(i, k, *matrix);
            assert(ind >= 0);
            sum += matrix->shared_mem_ptr[ind];
        }
        printf("Row %d: sum == %lf\n", i, sum);
        return;
    }
    fprintf(stderr, "Wrong input.\n");
}

void Swap(Matrix* matrix)
{
    int x1, x2;
    if (x1 < 0 || x2 < 0 ||  x2 >= matrix->row || x1 >= matrix->row)
    {
        fprintf(stderr, "Wrong parametrs.\n");
        return;
    }
    for (int i = 0; i < matrix->col; ++i)
    {
        double Tmp;
        int ind1 = GetIndex(x1, i, *matrix);
        int ind2 = GetIndex(x2, i, *matrix);
        assert(ind1 >= 0 && ind2 >= 0);
        Tmp = matrix->shared_mem_ptr[ind1];
        matrix->shared_mem_ptr[ind1] = matrix->shared_mem_ptr[ind2];
        matrix->shared_mem_ptr[ind2] = Tmp;
    }
    printf("Swaped row %d and %d\n", x1, x2);
}

void Set(Matrix* matrix)
{
    int x, y;
    double Value;
    int ind = GetIndex(x, y, *matrix);

    if (ind == -1)
    {
        fprintf(stderr, "Wrong parametrs.\n");
    }
    else
    {
        matrix->shared_mem_ptr[ind] = Value;
        printf("Set matrix[%d][%d] = %lf\n", x, y, Value);
    }
}

