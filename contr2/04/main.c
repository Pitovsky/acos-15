#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

int *stringIndex;
int *wordIndex;

int assignmentIndex;

const char *data;
ssize_t fileSize;

void *countTask(void *arg)
{
    int *num = arg;
    char c;
    char *string = (char*)malloc(assignmentIndex*sizeof(char));
    strncpy(string, data + (*num) * assignmentIndex, assignmentIndex);
    
    for (int i = 0; i < assignmentIndex; i++)
    {
        if (string[i] == '\n')
        {
            stringIndex[(*num)]++;
        }
    }
    
    if (strlen(string))
    {
        stringIndex[(*num)]++;
    }
    
    int index, flag = 1;
    size_t current = 0, strLen = strlen(string);
    while (current < strLen)
    {
        c = string[current];
        if ( (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')  || (c >= 'A' && c <= 'Z') || (c == '_'))
        {
            if (flag == 0)
                index++;
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        ++current;
    }
    wordIndex[(*num)] += index;
    
    free(string);
    return NULL;
}


int main(int argc, const char **argv)
{
    setbuf(stdout, 0);
    stringIndex = NULL;
    wordIndex = NULL;
    
    int file_fd;
    if ((file_fd = open(argv[1], O_RDWR)) < 0)
    {
        perror("can't open a file");
        exit(EXIT_SUCCESS);
    }
    struct stat entryInfo;
    lstat(argv[1], &entryInfo);
    fileSize = entryInfo.st_size;
    
    
    data = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);
    
    
    int threadsQTY = atoi(argv[2]);
    
    assignmentIndex = (int)fileSize/threadsQTY;
    
    wordIndex = (int*)malloc(threadsQTY*sizeof(int));
    stringIndex = (int*)malloc(threadsQTY*sizeof(int));
    
    pthread_t *threads = (pthread_t *)malloc(threadsQTY * sizeof(pthread_t));
    
    for (int i = 0; i < threadsQTY; ++i)
    {
        wordIndex[i] = 0;
        stringIndex[i] = 0;
        pthread_create(&threads[i], NULL, countTask, (void*)&i);
    }
    
    int stringsQTY = 0, wordsQTY = 0;
    for (int i = 0; i < threadsQTY; ++i)
    {
        stringsQTY += stringIndex[i];
        wordsQTY += wordIndex[i];
    }
    
    int remain = (int) (fileSize - fileSize/threadsQTY);
    int stringRemain = 0;
    int wordsRemain = 0;
    
    if (remain > 0)
    {
        char c;
        char* str = (char*)malloc(remain * sizeof(char));
        strncpy(str, data + threadsQTY * assignmentIndex, remain);
        
        for (int i = 0; i < remain; i++)
        {
            if (str[i] == '\n')
            {
                stringRemain++;
            }
        }
        if (strlen(str)!=0)
        {
            stringRemain++;
        }
        
        int counter, flag = 1;
        
        size_t currentPosition = 0, stringSize = strlen(str);
        
        while (currentPosition < stringSize)
        {
            c = str[currentPosition];
            
            if ( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_') || (c>='0' && c<='9') )
            {
                if (flag == 0)
                {
                    counter++;
                }
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            ++currentPosition;
        }
        wordsRemain += counter;
        free(str);
    }
    
    stringsQTY += stringRemain;
    wordsQTY += wordsRemain;
    
    printf("\nStrings : %d\n Words : %d\n", stringsQTY, wordsQTY);
    
    munmap((void *)data, fileSize);
    close(file_fd);
    
    return 0;
}
