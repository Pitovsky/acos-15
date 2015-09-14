#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int filesize;
int thrCount;
int wordCount = 0;
char* infile;

static void* wordCounter(void* cnmb)
{
    int nmb = *(int*)cnmb;
    int i = 0;
    int inword = 0;
    //printf("thr nmb %d: from %d to %d\n", nmb, nmb*filesize/thrCount, (nmb + 1)*filesize/thrCount);
    for (i = nmb*filesize/thrCount; i < (nmb + 1)*filesize/thrCount; ++i)//кажется, тут проблемы когда не делится на цело
    {
        //формально, слово определялось у нас по другому.
        if (inword == 1 && infile[i] <= ' ')
        {
            inword = 0;
        }
        else if (inword == 0 && infile[i] > ' ')
        {
            inword = 1;
            wordCount++;
        }
    }
    if (inword == 1 && nmb < thrCount - 1 && infile[(nmb + 1)*filesize/thrCount] > ' ')//идея правильная
        wordCount--; //потому что след.поток посчитает его еще раз
    pthread_exit(NULL);
}
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: file threadcount\n");
        return 1;
    }

    thrCount = atoi(argv[2]);

    struct stat st;
    stat(argv[1], &st);
    filesize = st.st_size;
    int fd = open(argv[1], O_RDONLY);
    if (filesize <= 0 || fd < 0)
    {
        fprintf(stderr, "Error: cannot open this file\n");
        return 2;
    }
    infile = (char*)mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
    int i = 0;
    pthread_t counterThr[thrCount];
    int nmbs[thrCount];
    for (i = 0; i < thrCount; ++i)
        nmbs[i] = i;
    for (i = 0; i < thrCount; ++i)
    {
        pthread_create(&counterThr[i], NULL, wordCounter, (void*)(&nmbs[i]));
    }

    for (i = 0; i < thrCount; ++i)//родительский поток в таких ситуациях тоже можно занять: чего добру проподать
    {
        pthread_join(counterThr[i], NULL);
    }
    printf("%d\n", wordCount);

    return 0;
}
