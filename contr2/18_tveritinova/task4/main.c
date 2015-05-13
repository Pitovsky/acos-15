#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

void thread_func(void* _thread_task)
{
    printf("in thread\n");
    int** thread_task = (int**) _thread_task;
    if (*thread_task == NULL)
    {
        printf("thread task is empty\n");
    }
    //printf("%zu", *thread_task);
    char just_to_test = ((char*) *thread_task)[0];
    //int file_start = (*thread_task)[0];
    printf("try to init file_addr\n");
    //char* file_addr = ((char**) (&((*thread_task)[4])))[0];
    //char* file_addr = (char*) &thread_task[4];
    char* file_addr = ((char**) &(thread_task[4]))[0];
    printf("done\n");
    int file_start = (*thread_task)[0];
    printf("done\n");
    int file_end = (*thread_task)[1];
    printf("done\n");
    int word_count = 0;
    int string_count = 0;
    if (file_addr == NULL)
    {
        printf("file_addr is empty\n");
    }
    printf("hello for\n");
    for (int i = file_start; i <= file_end; i++)
    {
        if (file_addr[i] == ' ')//вообщето, слова не совсем так нужно считать
        {
            printf("in if\n");
            if ((i != 0) && (file_addr[i - 1] != ' ') && (file_addr[i - 1] != '\n'))
            {
                ++word_count;
            }
        }
        if (file_addr[i] == '\n')
        {
            printf("in if\n");
            ++string_count;
        }
    }
    //int* result = (int*) malloc(sizeof(int) * 2);
    (*thread_task)[2] = word_count;
    (*thread_task)[3] = string_count;
}

int main(int argc, char** argv)
{
    struct stat file_stat;
    int fd = open(argv[1], O_RDWR);
    fstat(fd, &file_stat);
    int thread_count = atoi(argv[2]);
    //int thread_task[5];
    char* file_addr = (char*) mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_addr == MAP_FAILED)
    {
        perror("mmap");
    }
    //thread_task[2] = file_addr;
    for (int i = 0; i < thread_count; i++)
    {
        pthread_t thread;
        int* thread_task = (int*) malloc(sizeof(int) * 4 + sizeof(char*));
        thread_task[0] = (file_stat.st_size / thread_count) * i;
        if (i == thread_count - 1)
        {
            thread_task[1] = file_stat.st_size - 1;
        }
        else
        {
            thread_task[1] = (file_stat.st_size / thread_count) * (i + 1) - 1;
        }
        //&thread[4] = (int*) file_addr;
        ((char**) &(thread_task[4]))[0] = file_addr;
        if (pthread_create(&thread, NULL, thread_func, (void*) &thread_task) < 0)
        {
            perror("pthread_create");
        }
        //printf("thread %d returned: %d - word, %d - string\n", i, thread_task[2], thread_task[3]); 
        pthread_join(thread, NULL);// у вас прогрмма последовательная
        printf("thread %d returned: %d - word, %d - string\n", i, thread_task[2], thread_task[3]);
    }
    munmap(file_addr, file_stat.st_size); 
}
