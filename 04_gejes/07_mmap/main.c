#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int newFile(const char* fileName, int size)
{
    FILE* newFile = fopen(fileName, "wb");
    double* zeroArr = (double*)malloc(size);
    memset(zeroArr, 0.0, size);
    fwrite(zeroArr, sizeof(double), size, newFile);
    free(zeroArr);
    fclose(newFile);
    return size*sizeof(double);
}
int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s filename columnCount [rowCount]\n", argv[0]);
        return 1;
    }
    int colCount = atoi(argv[2]);
    char* fileName = argv[1];

    struct stat st;
    stat(fileName,&st);
    int fileSize = st.st_size;
    if (fileSize <= 0) //creating a new file
    {
        fileSize = newFile(fileName, colCount);
    }

    int rowCount = 1;
    if (argc > 3)
    {
        rowCount = atoi(argv[3]);
    }
    else
    {
        rowCount = fileSize/(sizeof(double)*colCount);
    }
    printf("open matrix: %d x %d\n", colCount, rowCount);

    if (fileSize < sizeof(double)*colCount*rowCount)
    {
        fileSize = newFile(fileName, colCount*rowCount);
    }

    int fd = open(fileName, O_RDWR | O_CREAT, 0666);
    double* beginPos = (double*)mmap(NULL, sizeof(double)*colCount*rowCount, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    char com[10] = {'h','e','l','p', 0, 0, 0, 0, 0, 0};

    while (strcmp(com, "exit") != 0)
    {
        if (strcmp(com, "getinfo") == 0)
        {
            printf("Info:\n\t%d columns,\t%d rows\n", colCount, rowCount);
        }
        else if (strcmp(com, "out") == 0)
        {
            double* i = beginPos;
            for (i = beginPos; i < beginPos + colCount*rowCount; ++i)
            {
                printf(" %lf", *i);
                if ((i - beginPos)%colCount == colCount - 1)
                    printf("\n");
            }
        }
        else if (strcmp(com, "get") == 0)
        {
            int x = 0;
            int y = 0;
            scanf("%d", &x);
            scanf("%d", &y);
            printf("%lf\n", *(beginPos + y*colCount + x));
        }
        else if (strcmp(com, "set") == 0)
        {
            int x = 0;
            int y = 0;
            double value = 0;
            scanf("%d", &x);
            scanf("%d", &y);
            scanf("%lf", &value);
            *(beginPos + y*colCount + x) = value;
        }
        else if (strcmp(com, "sumcol") == 0)
        {
            int x = 0;
            scanf("%d", &x);
            double sum = 0;
            int i = 0;
            for (i = 0; i < rowCount; ++i)
                sum = sum + *(beginPos + i*colCount + x);
            printf("%lf\n", sum);
        }
        else if (strcmp(com, "sumrow") == 0)
        {
            int y = 0;
            scanf("%d", &y);
            double sum = 0;
            int i = 0;
            for (i = 0; i < colCount; ++i)
                sum = sum + *(beginPos + y*colCount + i);
            printf("%lf\n", sum);
        }
        else if (strcmp(com, "swaprow") == 0)
        {
            int y1 = 0;
            int y2 = 0;
            scanf("%d", &y1);
            scanf("%d", &y2);
            int i = 0;
            for (i = 0; i < colCount; ++i)
            {
                double tmp = *(beginPos + y1*colCount + i);
                *(beginPos + y1*colCount + i) = *(beginPos + y2*colCount + i);
                *(beginPos + y2*colCount + i) = tmp;
            }
        }
        else if (strcmp(com, "transpose") == 0)
        {
            int i = 0;
            int j = 0;
            if (colCount != rowCount)
            {
                double tmpMtx[colCount][rowCount];
                for (i = 0; i < colCount; ++i)
                    for (j = 0; j < rowCount; ++j)
                        tmpMtx[i][j] = *(beginPos + j*colCount + i);
                int tmp = colCount;
                colCount = rowCount;
                rowCount = tmp;
                for (i = 0; i < rowCount; ++i)
                    for (j = 0; j < colCount; ++j)
                        *(beginPos + i*colCount + j) = tmpMtx[i][j];
            }
            else
            {
                for (i = 0; i < rowCount; ++i)
                    for (j = i; j < colCount; ++j)
                    {
                        double tmp = *(beginPos + i*colCount + j);
                        *(beginPos + i*colCount + j) = *(beginPos + j*colCount + i);
                        *(beginPos + j*colCount + i) = tmp;
                    }
            }
        }
        else if (strcmp(com, "help") == 0)
        {
            printf("exit - last command\n");
            printf("getinfo - print count of columns and rows\n");
            printf("get x y - print value from (x;y)\n");
            printf("out - print ALL matrix, be attentive with size!\n");
            printf("set x y v - set value from (x;y) as v\n");
            printf("sumcol x, sumrow y - print sum from column x/row y\n");
            printf("swaprow i j, swapcol i j - swap rows/columns i and j\n");
            printf("transpose - transpose matrix, matrix should be square\n");
        }
        else
        {
            printf("Error: command \"%s\" not found\n", com);
        }
        printf("next command:");
        scanf("%s", com);
    }

    munmap((void*)beginPos, sizeof(double)*colCount*rowCount);

    return 0;
}
