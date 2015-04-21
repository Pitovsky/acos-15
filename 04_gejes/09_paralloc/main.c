#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
/*
union semun
{
    int val;
    struct semid_ds* buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int binsem_init(int semid)
{
    union semun argument;
    unsigned short values(1);
    values[0] = 1;
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}*/

int Vfunc(char* outputStr, char* inputBegin)
{
        int i = 0;
        int j = 0;
        int len = 0;
        while (*(inputBegin + i) != '\n')
        {
            char pch = *(inputBegin + i);
            if (pch >= 'a' && pch <= 'z')
            {
                pch = pch - 'a' + 'A';
                j = 0;
                while (*(outputStr + j) != 0)
                    ++j;
                *(outputStr + j) = pch;
                len = len + 1;
            }
            else if (pch >= 'A' && pch <= 'Z')
            {
                pch = pch - 'A' + 'a';
                while (*(outputStr + j) != 0)
                    ++j;
                *(outputStr + j) = pch;
                *(outputStr + j + 1) = pch;
                len = len + 2;
            }
            else
            {
                while (*(outputStr + j) != 0)
                    ++j;
                *(outputStr + j) = pch;
                len = len + 1;
            }
            ++i;
        }
        return len + 1;
}

int main(int argc, char** argv)
{
    int sonCount = 2;
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s filename [soncount]\n", argv[0]);
        return 1;
    }
    if (argc == 3)
        sonCount = atoi(argv[2]);

    struct stat st;
    stat(argv[1],&st);
    int fileSize = st.st_size;
    if (fileSize <= 0)
    {
        fprintf(stderr, "Error: cannot read this file: %s\n", argv[1]);
        return 2;
    }

    int infd = open(argv[1], O_RDONLY, 0666);
    char* inputBegin = (char*)mmap(NULL, fileSize, PROT_READ, MAP_SHARED, infd, 0);
    int outhfd = shm_open("/outputheader", O_RDWR | O_CREAT,  0666);
    int outfd = shm_open("/output", O_RDWR | O_CREAT,  0666);
    int hfd = shm_open("/handlers", O_CREAT | O_RDWR,  0666);
    int semfd = shm_open("/sems", O_RDWR | O_CREAT,  0666);
    if (semfd < 0 || hfd < 0 || outfd < 0 || outhfd < 0)
    {
        fprintf(stderr, "Error: shm_open failed\n");
        return 3;
    }

    int fileHeight = 0;
    int i;
    for (i = 0; i < fileSize; ++i)
        if (*(inputBegin + i) == '\n')
            ++fileHeight;
    printf("size: %d\theight: %d\n", fileSize, fileHeight);

    ftruncate(hfd, fileHeight*sizeof(int));
    int* handlersBegin = (int*)mmap(NULL, fileHeight*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, hfd, 0);

    for (i = 0; i < fileHeight; ++i)
    {
        *(handlersBegin + i) = i;
    }

    ftruncate(outfd, fileSize*2*sizeof(char));
    char* outputBegin = (char*)mmap(NULL, fileSize*2*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, outfd, 0);

    int outheaderLength = sizeof(char*)*(1 + 2*fileHeight); //1 for first free position and 2*fileHeight for pairs begin-end of strings
    ftruncate(outhfd, outheaderLength*sizeof(char*));
    char** outheaderBegin = (char**)mmap(NULL, outheaderLength*sizeof(char*), PROT_READ | PROT_WRITE, MAP_SHARED, outhfd, 0);
    *outheaderBegin = outputBegin;
    for (i = 1; i < outheaderLength; ++i)
        *(outheaderBegin + i) = NULL;

    ftruncate(semfd, 2*sizeof(sem_t));
    sem_t* semh = (sem_t*)mmap(NULL, 2*sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, semfd, 0);
    sem_t* semouth = semh + 1;
    sem_init(semh, 1, 1);
    sem_init(semouth, 1, 1);

    pid_t par;
    for (i = 0; i < sonCount; ++i)
        if (par != 0)
            par = fork();

    if (par == 0)
    {
        while (1)
        {
            i = 0;

            sem_wait(semh); //get handler
            while (i < fileHeight && *(handlersBegin + i) == -1) //-1 instead of 0, because 0 is first string
                ++i;
            if (i != fileHeight)
                *(handlersBegin + i) = -1;
            sem_post(semh);

            int nowStr = i;
            if (nowStr == fileHeight)
                return 0;

            int i = 0;
            char* nowStrBegin = inputBegin;
            while (i < nowStr) //find neccessary string
            {
                if (*nowStrBegin == '\n')
                    ++i;
                ++nowStrBegin;
            }
            char outputStr[512];
            memset(outputStr, 0, 512);
            int outLen = Vfunc(outputStr, nowStrBegin);

            //printf("%d: %s\n", nowStr, outputStr);

            sem_wait(semouth); //change header
            *(outheaderBegin + 1 + nowStr*2) = *outheaderBegin;
            *(outheaderBegin + 1 + nowStr*2 + 1) = (*outheaderBegin) + outLen;
            strncpy(*outheaderBegin, outputStr, outLen);
            *outheaderBegin = *outheaderBegin + outLen;
            sem_post(semouth);
        }
    }

    else
    {
        for (i = 0; i < sonCount; ++i)
            wait();
        printf("-------------------sons ended\n");

        for (i = 0; i < fileHeight; ++i)
            printf("%s\n", *(outheaderBegin + 1 + i*2));
    }

    return 0;
}
