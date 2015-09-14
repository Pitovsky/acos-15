#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/mman.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t * thread;
int* connected;

struct info
{
    int beg;
    int end;
    char * file_pointer;
};

struct info* Info;
int sum_str = 0;
int sum_word = 0;

int count_str(char* fp, int beg, int end)
{
    int sum_s = 0;
    int i;
    for (i = beg; i < end; ++i)
    {
        if (*(fp + i) != ('\n' - '0'))
        {
            continue;
        }
        else ++sum_s;
    }
    return sum_s;
}

int count_Word(char * fp, int beg, int end)
{
    int count;
    int i;
    for (i = beg; i < end; ++i)
    {
        if ((*(fp + i) != ('\n' - '0')) || (*(fp + i) != (' ' - '0')))
        {
            continue;
        }
        else
        {
            while (((*(fp + i) == ('\n' - '0')) || (*(fp + i) == (' ' - '0'))) && (i < end))
            {
                continue;
                   ++i;
            }
            if (i < end) ++count;
        }
    }
    return count;
}

void* thr_func(void * arg)
{
    int count = (int)arg;
    int beg = Info[count].beg;
    int end = Info[count].end;
    connected[count] = 1;

    char* file_pointer = Info[count].file_pointer;
    sum_str += count_str(file_pointer, beg, end);
    sum_word += count_Word(file_pointer, beg, end);
}

int main(int argc, char** argv)
{
    int count_thr = atoi(argv[2]);
    thread = (pthread_t *)malloc(count_thr * sizeof(pthread_t));
    int fd = open(argv[1], O_RDWR);

    struct stat status;
    fstat(fd, &status);
    connected = (int*)calloc(count_thr, sizeof(int));
    Info = (struct info*)malloc(count_thr*sizeof(struct info));

    char * file_pointer = mmap(0, status.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    int i;
    for (i = count_thr; i > 0; --i)
    {
        pthread_mutex_lock(&mtx);//а зачем здесь. мьютекс тут в дргом метсе нужен
        int count = 0;
        while (connected[count])
        {
               ++count;
        }
        int beg, end;
        beg = count * count_thr;
        end = beg + (status.st_size / count_thr);
        Info[count].beg = beg;
        Info[count].end = end;
        Info[count].file_pointer = file_pointer;
        pthread_create(&thread[count], NULL, thr_func, (void*)count);
        pthread_detach(thread[count]);
        pthread_mutex_unlock(&mtx);
    }
    //нет обработки краевых эффектов
    //здесь у вас не корректно считается.

    printf("str_ sum: %d sum : %d\n", sum_str, sum_word );

    return 0;
}
