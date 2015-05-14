//
//  main.c
//  Merger
//
//  Created by Simon Fedotov on 01.05.15.
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


int main(int argc, const char * argv[]) {
    if(!strcmp(argv[1], "-m")) {
        char* FirstOutput = malloc(1024);
        char* SecondOutput = malloc(1024);
        FILE* First = popen(argv[2], "r");
        FILE* Second = popen(argv[3], "r");
        char* FirstValue = malloc(1024);
        char* SecondValue = malloc(1024);

        while (!feof(First) && !feof(Second)) {
            
            fscanf(First, "%s\t%s", FirstOutput, FirstValue);
            fscanf(Second, "%s\t%s", SecondOutput, SecondValue);
            if(!strcmp(FirstOutput, SecondOutput))
                printf("%s\t%s\t%s\t%s\n", FirstOutput, FirstValue, SecondOutput, SecondValue);
            else {
                if(atoi(FirstOutput) < atoi(SecondOutput)) {
                    while (!feof(First) && strcmp(FirstOutput, SecondOutput)) {
                        fscanf(First, "%s\t%s", FirstOutput, FirstValue);
                    }
                    if(feof(First))
                        break;
                    else
                        printf("%s\t%s\t%s\t%s\n", FirstOutput, FirstValue, SecondOutput, SecondValue);
    
                }
                else {
                    while (!feof(Second) && strcmp(FirstOutput, SecondOutput)) {
                        fscanf(Second, "%s\t%s", SecondOutput, SecondValue);
                    }
                    if(feof(Second))
                        break;
                    else
                        printf("%s\t%s\t%s\t%s\n", FirstOutput, FirstValue, SecondOutput, SecondValue);
                }
            }
        }
        
        while (!feof(First)) {
            fscanf(First, "%s\t%s", FirstOutput, FirstValue);
            printf("%s\t%s\n", FirstOutput, FirstValue);
        }
        while (!feof(Second)) {
            fscanf(Second, "%s\t%s", SecondOutput, SecondValue);
            printf("%s\t%s\n", SecondOutput, SecondValue);
        }

        
        
        
        printf("FIRST = %s\n", FirstOutput);
        printf("SECOND = %s\n", SecondOutput);
        
        pclose(First); pclose(Second);
    }
    
    
    else {
        int HowManyProcess = argc - 1;
        FILE* OurResult[HowManyProcess];
        memset(OurResult, 0, HowManyProcess * (sizeof(FILE*)));

        printf("ProcessNumber = %d\n", HowManyProcess);
        
        int OpenedOrNot[HowManyProcess];
        
        for (int i = 0; i < HowManyProcess; ++i)
            OpenedOrNot[i] = 1;
        
        

        
        for(int i = 0; i < HowManyProcess; ++i) {
            OurResult[i] = popen(argv[i + 1], "r");
            if(OurResult[i] == NULL) {
                perror("We've got an error while popening!");
                exit(1);
            }
        }
        
        
        char* Res = malloc(1 << 10);
        
        int Counter = HowManyProcess;
        while (Counter > 0) {
            for(int i = 0; i < HowManyProcess; ++i) {
                if(OpenedOrNot[i]) {
                    if(feof(OurResult[i])){
                        --Counter;
                        OpenedOrNot[i] = 0;
                        continue;
                    }
                    fgets(Res, 100, OurResult[i]);
                    Res[strlen(Res) - 1] = '\0';
                    if(strcmp(Res, ""))
                        printf("%s\t", Res);
                    else
                        printf("----End Of %d Process----\n", i);

                    
                   

                }
            }
            
            printf("\n");
            Res[0] = '\0';
        }


        for(int i = 0; i < HowManyProcess; ++i)
            pclose(OurResult[i]);
        
    }
    return 0;
}
