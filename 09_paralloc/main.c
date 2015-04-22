#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "stringMagic.h"

char SharedMemoryLabel[] = "ASDFGHJOIUYTR";

int *getEntryPoints(char *data)
{
    int *entryPoints = (int *)calloc(2, sizeof(int));
    for (int i = 0, j = 1; data[i]; i++)
    {
        if (data[i] == '\n')
        {
            j++;
            entryPoints = (int *)realloc(entryPoints, (j + 1) * sizeof(int));
            entryPoints[j - 1] = i + 1;
            entryPoints[j] = '\0';
        }
    }
    return entryPoints;
}

void processString()
{
    int sharedMemoryD = shm_open(SharedMemoryLabel, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (sharedMemoryD < 0)
    {
        perror("Can't shm_open");
        exit(1);
    }
    
    void *data = NULL;
    if ((data = mmap(NULL, 250, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryD, 0)) == MAP_FAILED)
    {
        perror("Child can't mmap a file");
        exit(1);
    }
    
    if (data == NULL)
    {
        perror("child data is nil");
    }
    
    pthread_mutex_t *mutex = data;
    pthread_mutex_lock(mutex);
    
    sleep(1);
    
    printf("yo\n");
    
    if(pthread_mutex_unlock(mutex) < 0)
    {
        perror("Child's pthread_mutex_unlock");
    }
}

int main(int argc, char **argv)
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    if (argc == 1)
    {
        printf("Invalid argument\n");
        exit(1);
    }
    
    char *filename = argv[1];
    int fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        perror("Can't open a file");
        exit(1);
    }
    
    struct stat fileInfo;
    lstat(filename, &fileInfo);
    
    void *data = NULL;
    if ((data = mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("Can't mmap a file");
        close(fd);
        exit(1);
    }
    
    int *points = getEntryPoints(data);
    
    int sharedMemoryD = shm_open(SharedMemoryLabel, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (sharedMemoryD < 0)
    {
        perror("Found shared memory");
        
        if (shm_unlink(SharedMemoryLabel) < 0)
        {
            perror("Can't shm_unlink");
            close(fd);
            exit(1);
        }
        
        sharedMemoryD = shm_open(SharedMemoryLabel, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (sharedMemoryD < 0)
        {
            perror("Can't shm_open");
            close(fd);
            exit(1);
        }
    }

    int pointsArraySize = 0;
    for (int i = 0; points[i] || i == 0; i++)
    {
        pointsArraySize++;
    }
    
    if (ftruncate(sharedMemoryD, sizeof(pthread_mutex_t) + pointsArraySize + 1 + strlen(data) + 2 * strlen(data)) < 0)
    {
        if (shm_unlink(SharedMemoryLabel) < 0)
        {
            perror("Can't shm_unlink");
            close(fd);
        }
        
        perror("ftruncate");
        close(fd);
        close(sharedMemoryD);
        exit(1);
    }
    
    void *sharedData = NULL;
    if ((sharedData = mmap(NULL, sizeof(pthread_mutex_t) + pointsArraySize + 1 + strlen(data) + 2 * strlen(data), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryD, 0)) == MAP_FAILED)
    {
        perror("Can't mmap a shared descriptor");
        exit(1);
    }
    
    memcpy(sharedData, &mutex, sizeof(pthread_mutex_t));
    
    pthread_t *threads = (pthread_t *)calloc(pointsArraySize, sizeof(pthread_t));
    for (int i = 0; i < pointsArraySize; i++)
    {
        if (!fork())
        {
            processString();
            exit(0);
        }
    }
    
    for (int i = 0; i < pointsArraySize; i++)
    {
        wait(0);
    }
    
    close(fd);
    close(sharedMemoryD);
    
    pthread_mutex_destroy(&mutex);
    
    free(points);
    shm_unlink(SharedMemoryLabel);
    
    return 0;
}