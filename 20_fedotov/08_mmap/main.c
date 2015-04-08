//
//  main.c
//  mmap
//
//  Created by Simon Fedotov on 03.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

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

void MakeSmth(double* FileAddress1, int* ColumnNumber, int* RowNumber, char* Command, int size) {
    
    if(strcmp(Command, "set") == 0) {
        int row, col;
        scanf("%d %d", &row, &col);
        double delta;
        scanf("%lf", &delta);
        *(FileAddress1 + (col - 1) + (row - 1) * (*ColumnNumber)) = delta;
    }
    
    else if(strcmp(Command, "get") == 0) {
        int row, col;
        scanf("%d %d", &row, &col);
        printf("Num At %d row and %d column is = %lf\n", row, col, *(FileAddress1 + (col - 1) + (row - 1) * (*ColumnNumber)));
        
    }
    
    else if(strcmp(Command, "exit") == 0) {
        exit(1);
    }
    
    else if(strcmp(Command, "getinfo") == 0) {
        printf("\n");
        printf("ColumNumber = %d\n", *ColumnNumber);
        printf("RowNumber = %d\n", (int)(size / (*ColumnNumber * sizeof(double))));
    }
    
    else if(strcmp(Command, "sum row") == 0) {
        int rowNum;
        scanf("%d", &rowNum);
        double result = 0;
        for(int i = (rowNum - 1) * (*ColumnNumber); i < rowNum * (*ColumnNumber); ++i) {
            result += *(FileAddress1 + i);
        }
        printf("\nresult is = %lf\n ", result);
    }
    
    else if(strcmp(Command, "sum col") == 0) {
        int colNum;
        scanf("%d", &colNum);
        double result = 0;
        for(int i = (colNum - 1); i < (*ColumnNumber) * (*RowNumber); i += (*ColumnNumber)) {
            result += *(FileAddress1 + i);
        }
        printf("\nresult is = %lf\n ", result);
    }
    
    else if(strcmp(Command, "swap") == 0) {
        int First, Second;
        scanf("%d %d", &First, &Second);
        for(int i = (First - 1) * (*ColumnNumber); i < First * (*ColumnNumber); ++i) {
            double tmp;
            tmp = *(FileAddress1 + i);
            *(FileAddress1 + i) = *(FileAddress1 + (Second - First) * (*ColumnNumber) + i);
            *(FileAddress1 + (Second - First) * (*ColumnNumber) + i) = tmp;
        }
    }
    
    else if (strcmp(Command, "transpose") == 0) {
        if(ColumnNumber == RowNumber) {
            for(int i = 0; i < *RowNumber; ++i) {
                for(int j = i; j < *ColumnNumber; ++j) {
                    double tmp;
                    tmp = *(FileAddress1 + i * (*ColumnNumber) + j);
                    *(FileAddress1 + i * (*ColumnNumber) + j) = *(FileAddress1 + j * (*ColumnNumber) + i);
                    *(FileAddress1 + j * (*ColumnNumber) + i) = tmp;
                }
            }
        }
        else {
            int temp = *RowNumber;
            int RowOld = *RowNumber;
            int ColOld = *ColumnNumber;
            RowNumber = ColumnNumber;
            *ColumnNumber = temp;
            double* NewMatrix = (double*)malloc(size);
            for(int i = 0; i < (*ColumnNumber); ++i) {
                for(int j = 0; j < (*RowNumber); ++j) {
                    NewMatrix[j * (*ColumnNumber) + i] = *(FileAddress1 + i * ColOld + j);
                }
            }
            for(int i = 0; i * sizeof(double) < size; ++i) {
                *(FileAddress1 + i) = NewMatrix[i];
            }
        }
    }
    
    else if(strcmp(Command, "get info") == 0) {
        printf("RowNumber is = %d\nColumnNumber is = %d\n", (*RowNumber), (*ColumnNumber));
    }

}


int main(int argc, const char * argv[]) {

    int ColumnNumber = 0;
    int RowNumber = 0;
    int OurFD;
    int size = 0;
    if(argc < 3) {
        printf("Incorrect input \n");
        exit(1);
    }

    OurFD = open(argv[1], O_RDWR | O_CREAT, 0777);
    if(OurFD < 0){
        printf("Error while opening file");
        printf("%d",errno);
    }
    struct stat statistic;
    if(fstat(OurFD, &statistic) < 0)
        printf("Can't get stats");
    size = statistic.st_size;
    
    if(argc == 3) {
        ColumnNumber = atoi(argv[2]);
        RowNumber = statistic.st_size / (ColumnNumber * sizeof(double));
        size = sizeof(double) * RowNumber * ColumnNumber;
        ftruncate(OurFD, size);
    }
    
    
    if(argc == 4) {
        ColumnNumber = atoi(argv[2]);
        RowNumber = atoi(argv[3]);
        size = sizeof(double) * RowNumber * ColumnNumber;
        ftruncate(OurFD, size);
    }
    
    
        double* FileAddress = (double*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, OurFD, 0);
        printf("%p", FileAddress);
        
        fstat(OurFD, &statistic);
        if(FileAddress == NULL)
            printf("\nERROR MAP\n");
    
    
        for(int i = 0; i * sizeof(double) < size; ++i) {
            *(FileAddress + i) = 10 * i;
        }
    
    
        if(munmap(FileAddress, size) < 0) {
            printf("bu");
        }
        
        
        double* ad = (double*)malloc(size);
        read(OurFD, ad, size);
        for(int i = 0; i * sizeof(double) < size; i++) {
            if(i % ColumnNumber == 0)
               printf("\n %lf ", *(ad + i));
            else
               printf(" %lf ", *(ad + i));
        }
        
        close(OurFD);

        
        int OurFD1 = open(argv[1], O_RDWR | O_CREAT, 0777);

        
        
        double* FileAddress1 = (double*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, OurFD1, 0);

        
        char * Command = malloc(255);
        gets(Command);
    
        MakeSmth(FileAddress1, &ColumnNumber, &RowNumber, Command, size);
    
        if( munmap(FileAddress1, size) < 0) {
            printf("bu2");
        }
        double* ad1 = (double*)malloc(size);
        read(OurFD1, ad1, size);
        for(int i = 0; i * sizeof(double) < size; i++) {
            if(i % ColumnNumber == 0)
                printf("\n %lf ", *(ad1 + i));
            else
                printf(" %lf ", *(ad1 + i));
            
        }
        
        
        close(OurFD1);
    
    printf("Hello, World!\n");
    return 0;
}
