//
//  main.c
//  task3
//
//  Created by Simon Fedotov on 19.03.15.
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



void ChildProcess(int PipeFD, int id)
{
    while (1) {
        char *MyString;
        int Data = read(PipeFD, &MyString, sizeof(MyString));
        FILE* OurFile = fopen(MyString, "r");
        int WordCounter = 0;
        char Check;
        int trueOrFalse = 0;
            while ((Check = getc(OurFile)) > 0) {
                if(!(((Check < 48 && Check != 45) || (Check > 57 && Check < 65) || (Check > 90 && Check < 97 && Check != 95) || Check > 122)) && !trueOrFalse) {
                    ++WordCounter;
                    trueOrFalse = 1;
                }
                else if(((Check < 48 && Check != 45) || (Check > 57 && Check < 65) || (Check > 90 && Check < 97 && Check != 95) || Check > 122)) {
                    trueOrFalse = 0;
                }
            }
            printf("%s\t %d \n", MyString, WordCounter);
    }
}

int main(int argc, const char * argv[]) {
    int ProcessNumber = atoi(argv[argc - 1]);
    int MyTasks[2];
    pipe(MyTasks);
    for (int i = 1; i < argc - 2; ++i) {
        write(MyTasks[1], &argv[i], sizeof(char**));
    }
    
    int ChildOrNot;
    for (int i = 0; i < ProcessNumber; ++i) {
        ChildOrNot = fork();
        if (ChildOrNot == 0) {
            ChildProcess(MyTasks[0], i);
        }
    }
    return 0;
}
