//
//  main.c
//  Task4
//
//  Created by Simon Fedotov on 07.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <math.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <pthread.h>

int RealAnswer[2];

void* ThreadFunction(char* Text) {
    int i = 0;
    int* DatAnswer = malloc(2 * sizeof(int));
    memset(DatAnswer, 0, 2 * sizeof(int));
    int WordCounter = 0;
    int StringCounter = 0;
    int trueOrFalse = 0;
    
    
    while (i < strlen(Text)) {
        if(!(((Text[i] < '0' && Text[i] != '-') || (Text[i] > '9' && Text[i] < 'A') || (Text[i] > 'z' && Text[i] < 'a' && Text[i] != '_') || Text[i] > 'z')) && !trueOrFalse) {
            ++WordCounter;
            trueOrFalse = 1;
        }
        else if(((Text[i] < '0' && Text[i] != '-') || (Text[i] > '9' && Text[i] < 'A') || (Text[i] > 'z' && Text[i] < 'a' && Text[i] != '_') || Text[i] > 'z')) {
            trueOrFalse = 0;
        }
        
        if(Text[i] == '\n')
            ++StringCounter;
        
        ++i;
    }
    
    
    ++StringCounter;
    DatAnswer[0] = WordCounter;
    DatAnswer[1] = StringCounter;
    
    RealAnswer[0] += WordCounter;
    RealAnswer[1] += StringCounter;
    
    pthread_exit(0);

    
    return NULL;
    
}


int main(int argc, const char * argv[]) {
    memset(RealAnswer, 0, 2 * sizeof(int));
    char * FilePath = malloc(1024);
    int ProcNumber;
    scanf("%s", FilePath);
    scanf("%d", &ProcNumber);
//    FilePath = argv[2];
//    ProcNumber = atoi(argv[3]);
    pthread_t Pthreads[ProcNumber];
    memset(Pthreads, 0, ProcNumber * sizeof(int));
    
    int OurFD = open(FilePath, O_RDWR | O_CREAT | S_IRWXU, 0777);
    if(OurFD < 0) {
        perror("Error While opening File");
        exit(1);
    }
    
    struct stat statistic;
    if(fstat(OurFD, &statistic) < 0)
        printf("Can't get stats");
    int FileSize = statistic.st_size;
    
    char* FileAddress = (char*)mmap(NULL, FileSize, PROT_READ | PROT_WRITE, MAP_SHARED, OurFD, 0);
    if(FileAddress == NULL) {
        perror("Error while mapping");
        exit(1);
    }
    
    printf("Bubu = %s\n", FileAddress);

    
    int Offset = 0;
    int Counter = 0;
        for(int i = 0; i < ProcNumber; ++i) {
            if(Offset >= FileSize) {
                printf("End Of File\n");
                break;
            }
            
            char* First = malloc(1 << 20);
            int j = 0;
            for(j = Offset; (j < Offset + FileSize  / ProcNumber /*|| First[j] != '\n'*/) && j < FileSize; ++j) {}
            strncpy(First, FileAddress + Offset, j - Offset + 1);
            Offset = j + 1;
            
            
            printf("bubuub\n");
            
            printf("First IS = %s\n\n", First);
            
            pthread_t DatThread;
            int bubu = pthread_create(&DatThread, NULL, ThreadFunction, First);
            Pthreads[Counter] = DatThread;
            if(bubu != 0)
                perror("Creating pthread error");
            
            ++Counter;
        }
    
    for(int i = 0; i < ProcNumber; ++i) {
        printf("bu %d\n", Pthreads[i]);
    }
    
    int i = 0;
    while (Pthreads[i] != 0 && i < ProcNumber) {
        int sha = pthread_join(Pthreads[i], NULL);
        if(sha != 0) {
            perror("Join Error");
            exit(1);
        }
        ++i;
    }
    
    printf("AND THE RESULT IS\nWordNum = %d\nStringNum = %d\n", RealAnswer[0], RealAnswer[1]);
    
    
    close(OurFD);
    munmap(FileAddress, FileSize);
    
    return 0;
}
