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

char* Splitter;

char *getString(FILE* OurFile)
{
    char *string = (char *)malloc(sizeof(char));
    int i = 0;
    char c;
    for (c = '\0'; (c=getc(OurFile))!='\n' && c != EOF; i++)
        (string = (char *)realloc(string, i+1), *(string + i) = c);
    *(string + i) = '\0';
    return string;
}



int main(int argc, const char * argv[]) {
    Splitter = malloc(255);
    printf("Do You Wanna Set a Splitter or '_Bu_' is good for you?\n Use Yes or No\n");
    char* Command = malloc(255);
    scanf("%s", Command);
    if(!strcmp(Command, "Yes")) {
        printf("Enter a Splitter: ");
        scanf("%s", Splitter);
    }
    else
        Splitter = "_Bu_";
    
    if(!strcmp(argv[1], "-m")) {
        char* FirstOutput = malloc(1024);
        char* SecondOutput = malloc(1024);
        char* FirstString = malloc(1024);
        char* SecondString = malloc(1024);
        char* FirstValue = malloc(1024);
        char* SecondValue = malloc(1024);
        FILE* First = popen(argv[2], "r");
        FILE* Second = popen(argv[3], "r");
        while (wait(0) >0) {}
        int SplitterSize = strlen(Splitter);
        
        fgets(FirstString, 1024, First);
        FirstString[strlen(FirstString) - 1] = '\0';
        fgets(SecondString, 1024, Second);
        SecondString[strlen(SecondString) - 1] = '\0';
        int FirstBound = strstr(FirstString, Splitter) - FirstString;
        int SecondBound = strstr(SecondString, Splitter) - SecondString;
        strncpy(FirstOutput, FirstString, FirstBound);
        strcpy(FirstValue, (char*)(FirstString + FirstBound + SplitterSize));
        strncpy(SecondOutput, SecondString, SecondBound);
        strcpy(SecondValue, SecondString + SecondBound + SplitterSize);

        while (!feof(First) && !feof(Second)) {
                while (strcmp(FirstOutput, SecondOutput) < 0 && !feof(First)) {
                printf("%s%s%s\n", FirstOutput, Splitter,  FirstValue);
                fgets(FirstString, 1024, First);
                FirstString[strlen(FirstString) - 1] = '\0';
                FirstBound = strstr(FirstString, Splitter) - FirstString;
                strncpy(FirstOutput, FirstString, FirstBound);
                strcpy(FirstValue, FirstString + FirstBound + SplitterSize);
            }
            if(feof(First))
                break;
            if(!strcmp(FirstOutput, SecondOutput)) {
                printf("%s%s%s%s%s%s%s\n", FirstOutput, Splitter, FirstValue, Splitter, SecondOutput, Splitter, SecondValue);
                fgets(FirstString, 1024, First);
                FirstString[strlen(FirstString) - 1] = '\0';
                fgets(SecondString, 1024, Second);
                SecondString[strlen(SecondString) - 1] = '\0';

                int FirstBound = strstr(FirstString, Splitter) - FirstString;
                int SecondBound = strstr(SecondString, Splitter) - SecondString;
                strncpy(FirstOutput, FirstString, FirstBound);
                strcpy(FirstValue, (char*)(FirstString + FirstBound + SplitterSize));
                strncpy(SecondOutput, SecondString, SecondBound);
                strcpy(SecondValue, SecondString + SecondBound + SplitterSize);
            }
            else {
                while (!feof(Second) && strcmp(FirstOutput, SecondOutput) > 0) {
                    printf("%s%s%s\n", SecondOutput, Splitter, SecondValue);
                    fgets(SecondString, 1024, Second);
                    SecondString[strlen(SecondString) - 1] = '\0';
                    SecondBound = strstr(SecondString, Splitter) - SecondString;
                    strncpy(SecondOutput, SecondString, SecondBound);
                    strcpy(SecondValue, SecondString + SecondBound + SplitterSize);
                }
                if(feof(Second))
                    break;
        
            }
        }
        
        while (!feof(First)) {
            fgets(FirstString, 1024, First);
            FirstString[strlen(FirstString) - 1] = '\0';
            int FirstBound = strstr(FirstString, Splitter) - FirstString;
            strncpy(FirstOutput, FirstString, FirstBound);
            strcpy(FirstValue, FirstString + FirstBound + SplitterSize);
            printf("%s%s%s\n", FirstOutput, Splitter, FirstValue);
        }
        while (!feof(Second)) {
            fgets(SecondString, 1024, Second);
            SecondString[strlen(SecondString) - 1] = '\0';
            int SecondBound = strstr(SecondString, Splitter) - SecondString;
            strncpy(SecondOutput, SecondString, SecondBound);
            strcpy(SecondValue, SecondString + SecondBound + SplitterSize);
            printf("%s%s%s\n", SecondOutput, Splitter, SecondValue);
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
                        printf("%s%s", Res, Splitter);
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
