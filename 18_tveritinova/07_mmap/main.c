#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int col_cnt = 0;
int row_cnt = 0;

void my_exit(double* file_addr)
{
    munmap(file_addr, col_cnt * row_cnt * sizeof(double));
}

double* get(double* file_addr, int x, int y)
{
    if ((x <= 0) || (x > row_cnt) || (y <= 0) || (y > col_cnt))
    {
       return NULL; 
    }
    else
    {
        return &(file_addr[(x - 1) * col_cnt + y]);
    }
}

void set(double* file_addr, int x, int y, double val)
{
    if ((x <= 0) || (x > row_cnt) || (y <= 0) || (y > col_cnt))
    {
        printf("incorrect argument, try again\n");
    }
    else
    {
        file_addr[(x - 1) * col_cnt + y] = val;
    }
}

double* sum_row(double* file_addr, int row)
{
    if ((row <= 0) || (row > row_cnt))
    {
        return NULL;
    }
    else
    {
        double* result = (double*) malloc(sizeof(double));
        *result = 0.0;
        double* get_res;
        for (int i = 1;i <= col_cnt;i++)
        {
            get_res = get(file_addr, row, i);
            *result += (get_res == NULL? 0: *get_res);
        }
        return result;
    }
}

double* sum_col(double* file_addr, int col)
{   
     if ((col <= 0) || (col > col_cnt))
     {
         return NULL;
     }
     else
     {
         double* result = (double*) malloc(sizeof(double));
         *result = 0.0;
         double* get_res;
         for (int i = 1;i <= row_cnt;i++)
         {
             get_res = get(file_addr, i, col);
             *result += (get_res == NULL? 0: *get_res);
         }   
         return result;
     }
}

void swap(double* file_addr, int one, int two)
{
    if ((one <= 0) || (one > row_cnt) || (two <= 0) || (two > row_cnt))
    {
        printf("incorrect argument, try again\n");
    }
    else
    {
        double swap; 
        double* get_res;
        if (one > two)
        {
            int a = one;
            one  = two;
            two = a;
        }
        for (int i = 1;i <= col_cnt;i++)
        {
            get_res = get(file_addr, one, i);
            swap = (get_res == NULL? 0: *get_res);
            get_res = get(file_addr, two, i);
            set(file_addr, one, i, (get_res == NULL? 0: *get_res));
            set(file_addr, two, i, swap);
        }
    }   
}
void getinfo()
{
    printf("number row:%d    number col:%d\n", col_cnt, row_cnt);
}

void transpose(double* file_addr)
{
    double swap;
    int buf_size = col_cnt >= row_cnt ? col_cnt : row_cnt;
    double buf[col_cnt][row_cnt];
    double* stream;
    for (int i=0;i < col_cnt; i++)
    {
        for (int j=0;j < row_cnt; j++)
        {
            buf[i][j] = *get(file_addr, j + 1, i + 1);
        }
    }
    int swap_ = col_cnt;
    col_cnt = row_cnt;
    row_cnt = swap_;
    for (int counter_row = 1;counter_row <= row_cnt;counter_row++)
    {
        for (int counter_col = 1;counter_col <= col_cnt;counter_col++)
        {
            set(file_addr, counter_row, counter_col, buf[counter_row - 1][counter_col - 1]);
        }
    }
}

void print(double* file_addr)
{
    for (int counter_row = 1;counter_row <= row_cnt;counter_row++)
    {
        for (int counter_col = 1;counter_col <= col_cnt;counter_col++)
        {
            if (get(file_addr, counter_row, counter_col) != NULL)
            {
                printf("%lf  ", *get(file_addr, counter_row, counter_col));
            }
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    col_cnt = atoi(argv[2]);
    int fd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1)
    {
        fprintf(stderr, "can't do open\n");
    }
    struct stat filestat;
    fstat(fd, &filestat);
    //printf("%lld\n", filestat.st_size);
    if ((argc == 3)  && (filestat.st_size == 0))
    {
        lseek(fd, 0, SEEK_END);
        lseek(fd, col_cnt * sizeof(double) - 1, SEEK_CUR);
        write(fd, "", 1);
        printf("allocated %d byte\n", col_cnt * sizeof(double));
    }
    if (argc == 4)
    {
        row_cnt = atoi(argv[3]);
        if (filestat.st_size < row_cnt * col_cnt * sizeof(double))
        {
            lseek(fd, 0, SEEK_END);
            lseek(fd, row_cnt * col_cnt * sizeof(double) - 1, SEEK_CUR);
            write(fd, "", 1);
        }
        if (filestat.st_size > row_cnt * col_cnt * sizeof(double))
        {
            ftruncate(fd, row_cnt * col_cnt * sizeof(double));
        }
    }
    else
    {
        row_cnt = filestat.st_size / (col_cnt * sizeof(double));
        if (row_cnt == 0)
        {
            row_cnt = 1;
        }
        if (row_cnt * col_cnt * sizeof(double) > filestat.st_size)
        {
            printf("allocated %lld byte\n", row_cnt * col_cnt * sizeof(double) - filestat.st_size);
        }
        if (row_cnt * col_cnt * sizeof(double) < filestat.st_size)
        {
            printf("free %lld byte\n", filestat.st_size - row_cnt * col_cnt * sizeof(double));
        }
        ftruncate(fd, row_cnt * col_cnt * sizeof(double));
    }
    printf("column count: %d    row count: %d\n", col_cnt, row_cnt);
    fstat(fd, &filestat);
    printf("file size: %lld\n",filestat.st_size);
    double* file_addr = (double*) mmap(0, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_addr == -1)
    {
        fprintf(stderr, "can't do mmap\n");
    }
    else
    {
        char command[10];
        int cmd_key;
        int x,y;
        int er;
        double val;
        while (1)
        {
            print(file_addr);
            scanf("%s", command);
            cmd_key = 0;
            if (strcmp(command, "exit") == 0)
            {
                my_exit(file_addr);
                break;
                cmd_key = 1;
            }
            if (strcmp(command, "getinfo") == 0)
            {
                getinfo();
                cmd_key = 1;
            }
            if (strcmp(command, "get") == 0)
            {
                er = scanf("%d",&x);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d",&x);
                }
                er = scanf("%d",&y);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d", &y);
                }
                if (get(file_addr, x, y) != NULL)
                {
                    printf("%lf\n",*get(file_addr, x, y));
                }
                else
                {
                    printf("syntax error, try again\n");
                }
                cmd_key = 1;
            }
            if (strcmp(command,"set") == 0)
            {
                er = scanf("%d",&x);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d",&x);
                }
                er = scanf("%d",&y);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d", &y);
                }
                er = scanf("%lf",&val);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%lf", &val);
                }
                set(file_addr, x, y, val);
                //printf("add %lf to row %d col %d\n", val, x, y);
                cmd_key = 1;
            }
            if (strcmp(command, "sum") == 0)
            {
                scanf("%s",command);   
                if((strcmp(command, "row") != 0) && (strcmp(command, "col") != 0))
                {
                    printf("incorrect syntax, write again\n");
                    scanf("%s",command);
                }
                er = scanf("%d",&x);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d", &x);
                }
                if (strcmp(command,"row") == 0)
                {
                    if (sum_row(file_addr,x) != NULL)
                    {
                        printf("%lf\n",*sum_row(file_addr, x));
                    }
                    else
                    {
                        printf("syntax error, try again\n");
                    }
                }
                if (strcmp(command, "col") == 0)
                {
                    if (sum_col(file_addr, x) != NULL)
                    {
                        printf("%lf\n",*sum_col(file_addr, x));
                    }
                    else
                    {
                        printf("syntax error, try again\n");
                    }
                }
                cmd_key = 1;
            }
            if (strcmp(command, "swap") == 0)
            {
                er = scanf("%d", &x);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d", &x);
                }
                er = scanf("%d", &y);
                while (er != 1)
                {
                    printf("incorrect syntax, write again\n");
                    getchar();
                    er = scanf("%d", &y);
                }
                swap(file_addr, x, y);
                //printf("swaped row %d and row %d\n", x, y);
                cmd_key = 1;
            }
            if (strcmp(command, "transpose") == 0)
            {
                transpose(file_addr);
                printf("transposed\n");
                cmd_key = 1;
            }
            if (strcmp(command, "print") == 0)
            {
                print(file_addr);
                printf("printed\n");
                cmd_key = 1;
            }
            if (cmd_key == 0)
            {
                printf("command not found, write again\n");
            }
        }
    }
    close(fd);
    return 0;
}
