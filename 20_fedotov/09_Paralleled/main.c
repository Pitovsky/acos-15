//
//  main.c
//  Paralleled
//
//  Created by Simon Fedotov on 20.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

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


int Offset;

char* OurFunction(char* ToChange) {
    char* NewString = malloc(strlen(ToChange) * 2);
    for(int i = 0; i < strlen(ToChange); ++i) {
        if(ToChange[i] >= 'A' && ToChange[i] <= 'Z') {
            char* ToInsert = malloc(2);
            ToInsert[0] = ToChange[i] + 32;
            ToInsert[1] = ToChange[i] + 32;
            strcat(NewString, ToInsert);
        }
        else if(ToChange[i] >= 'a' && ToChange[i] <= 'z') {
            char* ToInsert = malloc(1);
            ToInsert[0] = ToChange[i] - 32;
            strcat(NewString, ToInsert);
        }
        else {
            char* ToInsert = malloc(1);
            ToInsert[0] = ToChange[i];
            strcat(NewString, ToInsert);
        }
    }
    return NewString;
}


int LineCounter(FILE* Path) {
    char DatChar;
    int LineNumber = 1;
    while ((DatChar = getc(Path)) != EOF) {
        if(DatChar == '\n')
            ++LineNumber;
    }
    return LineNumber;
}

void FillFirstZone(FILE* OurFile, int* OurArray) {
    int counter = 1;
    int LineNumber = 1;
    char DatChar;
    while ((DatChar = getc(OurFile)) != EOF) {
        if(DatChar == '\n') {
            OurArray[LineNumber] = counter;
            ++LineNumber;
        }
        ++counter;
    }
}



int main(int argc, const char * argv[]) {
    char* OurFile = malloc(255);
    scanf("%s", OurFile);
//    strcpy(OurFile, argv[1]);
    FILE* DatFile = fopen(OurFile, "rw");
    if(DatFile == NULL)
        perror("Error While opening File");
    
    struct stat OurStatistic;
    int resik = stat(OurFile, &OurStatistic);
    if(resik < 0)
        perror("Error while getting stats");
    
    int SizeOfFile = OurStatistic.st_size;
    int LineNumber = LineCounter(DatFile);
    
    fclose(DatFile);
    
    printf("linii%d", LineNumber);
    
    int StringsStarts[LineNumber];
    
    StringsStarts[0] = 1;
    DatFile = fopen(OurFile, "rw");
    if(DatFile == NULL)
        perror("Error While opening File");
    
    FillFirstZone(DatFile, StringsStarts);
    
    fclose(DatFile);

    printf("\n");
    for(int i = 0; i < LineNumber; ++i) {
        printf(" %d ", StringsStarts[i]);
    }
    printf(" lala\n");

    
    DatFile = fopen(OurFile, "rw");
    if(DatFile == NULL)
        perror("Error While opening File");
    
    
    int First = shm_open("/Users/semenfedotov/First.txt", O_RDWR | O_CREAT, 0777);
    int Second = open(OurFile,  O_RDWR, 0777);
    int Third = shm_open("/Users/semenfedotov/Third.txt",  O_RDWR | O_CREAT, 0777);
    if(First < 0)
        perror("First opening Error");
    if(Second < 0)
        perror("Second opening Error");
    if(Third < 0)
        perror("Third opening Error");
  
    ftruncate(Third, 2 * LineNumber * sizeof(int) + SizeOfFile * 2);
    ftruncate(Second, SizeOfFile);
    ftruncate(First, LineNumber * sizeof(int));
    
    dispatch_semaphore_t FirstZoneSemaphore = dispatch_semaphore_create(1);
    dispatch_semaphore_t ThirdZoneSemaphore = dispatch_semaphore_create(1);

    
    if(FirstZoneSemaphore == NULL) {
        perror("Error while opening FirstZoneSemaphore");
    }
    
    if(ThirdZoneSemaphore == NULL) {
        perror("Error while opening ThirdZoneSemaphore");
    }
    
    
    int* FirstZonePointer = (int*) mmap(NULL, LineNumber * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, First, 0);
    
    if(FirstZonePointer == MAP_FAILED)
        perror("First zone mapping error");

    
    char* SecondZonePointer =(char*) mmap(NULL, SizeOfFile, PROT_READ | PROT_WRITE, MAP_SHARED, Second, 0);
    
    if(SecondZonePointer == MAP_FAILED)
        perror("Second zone mapping error");
    
    void* ThirdZonePointer = mmap(NULL, SizeOfFile * 2 + 2 * LineNumber * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, Third, 0);
    
    if(ThirdZonePointer == MAP_FAILED)
        perror("Third zone mapping error");

    Offset = 2 * LineNumber * sizeof(int*) + 1;
    
    for(int i = 0; i < LineNumber; ++i) {
        *(FirstZonePointer + i) = StringsStarts[i];
        printf("%d ", *(FirstZonePointer + i));
    }
    printf("\n");//Не забывть про слешН
    
    printf("\n Ourfile =\n%s\n", SecondZonePointer);
    
    
    int ProcessNumber = LineNumber;
    
    while (ProcessNumber > 0) {
        int res = fork();
        if(res == 0) {
            int Counter = 0;
            int StartPosition = 0;
            dispatch_semaphore_wait(FirstZoneSemaphore, DISPATCH_TIME_FOREVER);
            while (*(FirstZonePointer + Counter) == 0 && Counter != LineNumber - 1) {
                ++Counter;
            }
            if(Counter == 0)
                StartPosition = 0;
            else
                StartPosition = *(FirstZonePointer + Counter);
            
            *(FirstZonePointer + Counter) = 0;
            dispatch_semaphore_signal(FirstZoneSemaphore);
            int WhatLine = Counter;
            char* DatString = malloc(1 << 20);
            Counter = 0;
            char* BeginPosition = (char*)(SecondZonePointer + StartPosition);
            while(*(BeginPosition + Counter) != '\n') {
                DatString[Counter] = *(BeginPosition + Counter);
                ++Counter;
            }
            strcpy(DatString, OurFunction(DatString));
            
            
            
            dispatch_semaphore_wait(ThirdZoneSemaphore, DISPATCH_TIME_FOREVER);
            for(int i = 0; i < strlen(DatString); ++i) {
                *(char*)(ThirdZonePointer + Offset + i) = DatString[i];
            }
            

            
            char * Result = malloc(1 << 20);
            Result = (char*)(ThirdZonePointer + 2 * LineNumber * sizeof(int*) + 1);
            printf("\n\nThe Result is = %s\n\n", Result);
            memset(Result, 0, strlen(Result));
            printf("\n");
            *(int*)(ThirdZonePointer + 2 * (WhatLine) * sizeof(int)) = Offset;
            Offset += strlen(DatString);
            
            *(int*)(ThirdZonePointer + 2 * (WhatLine) * sizeof(int) + sizeof(int)) = Offset;
            dispatch_semaphore_signal(ThirdZoneSemaphore);
            sleep(1);
            
            exit(1);
        }
        while (wait(0) > 0) {}
        int counter = 0;
        char ds;
//
//        Offset = *(int*)(ThirdZonePointer + 2 * (6 - ProcessNumber) * sizeof(int) + sizeof(int)) + 1;
        --ProcessNumber;
    }
    
    

    
  
    dispatch_release(FirstZoneSemaphore);
    dispatch_release(ThirdZoneSemaphore);
    munmap(FirstZonePointer, LineNumber * sizeof(int));
    munmap(SecondZonePointer, SizeOfFile);
    munmap(ThirdZonePointer, SizeOfFile * 2 + 2 * LineNumber * sizeof(int));
    printf("\nFirstZoneSem = %d\n", FirstZoneSemaphore);
    printf("\nThirdZoneSem = %d\n", ThirdZoneSemaphore);
    
   
//    printf("DatValue = %d", OurValue);
   int OutFirst = shm_unlink("/Users/semenfedotov/First.txt");
   int OutSecond = close(Second);
   int OutThird = shm_unlink("/Users/semenfedotov/Third.txt");
    if(OutFirst < 0)
        perror("First Unlink error");
    if(OutSecond < 0)
        perror("Second Closing error");
    if(OutThird < 0)
        perror("Third Unlink error");

    
    int Chird = shm_open("/Users/semenfedotov/Third.txt",  O_RDWR | O_CREAT, 0777);

    void* ChirdZonePointer = mmap(NULL, SizeOfFile * 2 + 2 * LineNumber * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, Chird, 0);
    
    if(ChirdZonePointer == MAP_FAILED)
        perror("Third zone mapping error");
    

    
    return 0;
}
