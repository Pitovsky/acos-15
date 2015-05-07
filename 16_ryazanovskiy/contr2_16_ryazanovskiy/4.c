#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>

typedef struct
{
    int words, lines;
} Stats_t;

/// global :(
int threadCount = 0;
size_t fileSize = 0;
char* filePointer = 0;

void* calculateWordStats(void* offset)
{
    Stats_t* stats = (Stats_t*) malloc(sizeof(Stats_t));

    stats->words = stats->lines = 0;

    int inWord = 0;
    for (int i = offset; i < fileSize / threadCount + offset; i++)
        if (filePointer[i] == '\n')
            stats->lines++;
        else if (inWord && filePointer[i] == ' ')
        {
            stats->words++;
            inWord = 0;
        }
        else if (!inWord && filePointer[i] != ' ')
            inWord = 1;

    return stats;
}


int main(int argc, char** argv)
{
    assert(3 == argc);

    char* fileName = argv[1];
    threadCount = atoi(argv[2]);

    int fd = open(argv[1], O_RDONLY);

    struct stat buf;
    fstat(fd, &buf);
    fileSize = buf.st_size;

    filePointer = (char*) mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);

    int* threads = malloc(sizeof(int) * threadCount);

    for (int thread = 0; i < threadCount; i++)
    {
        pthread_create(threads + thread)
    }

    close(fd);

    return 0;
}
