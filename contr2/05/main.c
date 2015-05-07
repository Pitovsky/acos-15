#include <stdio.h>
#include <pthread/pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "FileProcessing.h"

#define cs_t 0
#define co_t 1

typedef struct
{
    pthread_mutex_t *mutex;
    int number;
} Data;

void *worker(void *arg)
{
    Data *data = arg;
    
    while (1)
    {
        pthread_mutex_lock(data->mutex);
        
        printf("Enter a command: ");
        
        int cmd_t = -1;
        char filename[128] = {0};
        
        char comd[30] = {0};
        
        if (scanf("%s %s", comd, filename) == EOF)
        {
            pthread_mutex_unlock(data->mutex);
            return NULL;
        }
        
        if (strcmp("co", comd) == 0)
        {
            cmd_t = co_t;
        }
        else
        {
            cmd_t = cs_t;
        }
        
        pthread_mutex_unlock(data->mutex);
        
        unsigned long results = -1;
        if (cmd_t == cs_t)
        {
            results = cs(filename);
        }
        else if (cmd_t == co_t)
        {
            results = co(filename);
        }
        
        printf("\n%s %s %ld\n", cmd_t ? "co" : "cs", filename, results);
    }
}

int main(int argc, const char * argv[])
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    int threads_qty = atoi(argv[1]);
    
    pthread_t **threads = (pthread_t **)calloc(threads_qty, sizeof(pthread_t *));
    
    Data *data = (Data *)malloc(sizeof(Data));
    data->mutex = &mutex;
    
    for (int i = 0; i < threads_qty; i++)
    {
        threads[i] = (pthread_t *)calloc(1, sizeof(pthread_t));
        pthread_create(threads[i], NULL, worker, data);
    }
    
    for (int i = 0; i < threads_qty; i++)
    {
        if(pthread_join(*threads[i], NULL))
        {
            perror("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        free(threads[i]);
    }
    
    return 0;
}
