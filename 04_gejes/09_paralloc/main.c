#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>

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
        fprintf(stderr, "Usage: %s filename [count_of_child]\n", argv[0]);
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
    char* inputBegin = (char*)mmap(NULL, fileSize*sizeof(char), PROT_READ, MAP_SHARED, infd, 0);
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
    int* handlersBegin = (int*)mmap(NULL, fileHeight*sizeof(int) + 2*sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, hfd, 0);

    for (i = 0; i < fileHeight; ++i)
    {
        *(handlersBegin + i) = i;
    }

    ftruncate(outfd, fileSize*2*sizeof(char));
    char* outputBegin = (char*)mmap(NULL, fileSize*2*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, outfd, 0); //2*size because len(V(x)) <= 2*len(x)

    int outheaderLength = sizeof(char*)*(1 + 2*fileHeight); //1 for first free position and 2*fileHeight for pairs begin-end of strings
    ftruncate(outhfd, outheaderLength*sizeof(char*));
    char** outheaderBegin = (char**)mmap(NULL, outheaderLength*sizeof(char*), PROT_READ | PROT_WRITE, MAP_SHARED, outhfd, 0);
    *outheaderBegin = outputBegin;
    for (i = 1; i < outheaderLength; ++i)
        *(outheaderBegin + i) = NULL;

    ftruncate(semfd, 2*sizeof(sem_t));
    sem_t* semh = (sem_t*)(handlersBegin + fileHeight);
    sem_t* semouth = semh + 1;
    sem_init(semh, 1, 1);
    sem_init(semouth, 1, 1);

    pid_t par = 1;
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

            i = 0;
            char* nowStrBegin = inputBegin;
            while (i < nowStr) //find neccessary string
            {
                if (*nowStrBegin == '\n')
                    ++i;
                ++nowStrBegin;
            }
            i = 0;
            while ((nowStrBegin - inputBegin) + i < fileSize && nowStrBegin[i] != '\n')
                ++i;
            char outputStr[2*i];
            memset(outputStr, 0, 2*i);
            int outLen = Vfunc(outputStr, nowStrBegin);

            printf("%d: %s\n", nowStr, outputStr);

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
        printf("-------------------childs ended\n");

        for (i = 0; i < fileHeight; ++i)
            printf("%s\n", *(outheaderBegin + 1 + i*2));
    }

    munmap(outheaderBegin, outheaderLength*sizeof(char*));
    munmap(outputBegin, fileSize*2*sizeof(char));
    munmap(handlersBegin, fileHeight*sizeof(int));
    munmap(inputBegin, fileSize*sizeof(char));

    shm_unlink("/sems");
    shm_unlink("/handlers");
    shm_unlink("/output");
    shm_unlink("/outputheader");

    close(infd);

    return 0;
}
