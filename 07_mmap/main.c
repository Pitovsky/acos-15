#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "matrix.h"

void alertAboutWrongInput()
{
    printf("WRONG INPUT\nUSAGE:\nmatrix [filename] -r [row] and -c [column]\n");
    exit(1);
}

struct
{
    char *filename;
    long int rowQty;
    long int columnQty;
} NSBundle;

void processArguments(int argc, char **argv);
char *getString();

int main(int argc, char **argv)
{
    processArguments(argc, argv);
    int fd = open(NSBundle.filename, O_RDWR);
    if (fd < 0)
    {
        perror("Can't open a file");
        exit(1);
    }
    
    struct stat fileInfo;
    lstat(NSBundle.filename, &fileInfo);
    void *data;
    if ((data = (double*)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("Can't mmap a file");
        exit(1);
    }
    
    Matrix *matrix;
    if ((matrix = createMatrix(data, NSBundle.rowQty, NSBundle.columnQty)) == NULL)
    {
        perror("Can't configure matrix");
        exit(1);
    }
    printMatrix(matrix);
    
    while (1)
    {
        char *command = getString();
        
        if (!strcmp(command, "exit"))
            break;
        
        else if (!strcmp(command, "getinfo"))
        {
            printf("Rows : %ld, Columns : %ld\n", matrix->rowQty, matrix->columnQty);
        }
        
        else if (strstr(command, "get"))
        {
            long int row, column;
            
            int res = sscanf(command + 4, "%ld", &row);
            if (res == 0 || strstr(command + 4, " ") == NULL)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            res = sscanf(strstr(command + 4, " "), "%ld", &column);
            if (res == 0)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            printf("%lf\n", matrix->elements[row - 1][column - 1]);
        }
        
        else if (strstr(command, "set"))
        {
            long int row, column;
            
            int res = sscanf(command + 4, "%ld", &row);
            if (res == 0 || strstr(command + 4, " ") == NULL)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            res = sscanf(strstr(command + 4, " "), "%ld", &column);
            if (res == 0)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            printf("Enter a new number: ");
            scanf("%lf", &matrix->elements[row - 1][column - 1]);
        }
        
        else if (strstr(command, "sum row"))
        {
            long int row;
            
            int res = sscanf(command + 7, "%ld", &row);
            if (res == 0 || row == 0)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            printf("%lf\n", sumRow(matrix, row - 1));
        }
        
        else if (strstr(command, "sum col"))
        {
            long int column;
            
            int res = sscanf(command + 7, "%ld", &column);
            if (res == 0 || column == 0)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            printf("%lf\n", sumRow(matrix, column - 1));
        }
        
        else if (strstr(command, "swap"))
        {
            long int row1, row2;
            
            int res = sscanf(command + 5, "%ld", &row1);
            if (res == 0 || strstr(command + 5, " ") == NULL)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            res = sscanf(strstr(command + 5, " "), "%ld", &row2);
            if (res == 0)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            
            swap(matrix, row1 - 1, row2 - 1);
        }
        
        free(command);
    }
    
    close(fd);
    destroyMatrix(matrix);
    return 0;
}

void processArguments(int argc, char **argv)
{
    if (argc != 6)
        alertAboutWrongInput();
    
    for (size_t i = 1; i < argc; i++)
    {
        if (!strcmp("-r", argv[i]))
        {
            long int rowQty;
            if ((rowQty = atoi(argv[i+1])) == 0)
                alertAboutWrongInput();
            NSBundle.rowQty = rowQty;
            i++;
            continue;
        }
        else if (!strcmp("-c", argv[i]))
        {
            long int columnQty;
            if ((columnQty = atoi(argv[i+1])) == 0)
                alertAboutWrongInput();
            NSBundle.columnQty = columnQty;
            i++;
            continue;
        }
        else
        {
            NSBundle.filename = argv[i];
        }
    }
}

char *getString()
{
    char *string = (char *)malloc(sizeof(char));
    int i = 0;
    char c;
    for (c = '\0'; (c=getchar())!='\n' && c != EOF; i++)
        (string = (char *)realloc(string, i+1), *(string + i) = c);
    *(string + i) = '\0';
    return string;
}