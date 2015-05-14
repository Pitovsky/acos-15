//
//  main.c
//  PipesAndExec
//
//  Created by Simon Fedotov on 28.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <curl/curl.h>
#include <sys/pipe.h>


void FindReference(char* URL, int FileDescriptor) {
    char Cash[10];
    Cash[9] = '\0';
    int counter = 0;
    char MyChar;
    while (counter < strlen(URL)) {
        MyChar = URL[counter];
        Cash[counter % 9] = MyChar;
        
        if((Cash[counter % 9] == 'A' || Cash[counter % 9] == 'a') && Cash[(counter - 1) % 9] == '<') {
            counter += 2;
            
            char Check = ' ';
            while (Check != '"' && Check != '\0') {
                Check = URL[counter];
                Cash[counter % 9] = Check;
                ++counter;
            }
            
            
            Check = 0;
            if(Cash[(counter - 6) % 9] == 'h' && Cash[(counter - 5) % 9] == 'r' && Cash[(counter - 4) % 9] == 'e' && Cash[(counter - 3) % 9] =='f') {
                
                char* linkName = malloc(1 << 20);
                while (Check != '"' && Check != '#' && Check != '(' && Check != ')' && Check != '{' && Check != '}' && Check != ' ') {
                    
                    Check = URL[counter];
                    if(Check == '"' || Check == '#' || Check == '(' || Check == ')' || Check == '{' || Check == '}' || Check == ' ')
                        break;
                    
                    char* str = malloc(255);
                    str[0] = Check;
                    strcat(linkName, str);
                    ++counter;
                }
                
                if(strlen(linkName) != 0) {
//                    printf("\n%s\n  ", linkName);
                    write(FileDescriptor, linkName, strlen(linkName));
                    
                }
                --counter;
            }
            
        }
        
        ++counter;
    }

}



int main(int argc, const char * argv[]) {
    char* DatPipeName = malloc(1 << 10);
    DatPipeName = getenv("URLS_SRC");
    mkfifo(DatPipeName, S_IFIFO | 077);
    int DatPipeFD = open(DatPipeName, O_WRONLY);
    if(DatPipeFD < 0) {
        perror("Error while opening UrlsPipe");
        exit(1);
    }
    char* DatURL = malloc(255);
    while (strcmp(DatURL, "end")){
            scanf("%s", DatURL);
            write(DatPipeFD, DatURL, 255);
    }
        fclose(DatPipeFD);
    
    char* FIFO_Name = getenv("URLS_SRC");
    int fd = open(FIFO_Name, O_RDONLY);
    if(fd < 0) {
        mkfifo(FIFO_Name, S_IFIFO | 0777);
        fd = open(FIFO_Name, O_RDWR | O_CREAT | O_NONBLOCK);
        if(fd < 0) {
            perror("Error while opening pipe");
                exit(1);
        }
    }
    
    

    
    
    //Создаем Пайп с УРЛами
    int UrlPipe[2];
    pipe(UrlPipe);
    fcntl(UrlPipe[0], F_SETFL, O_NONBLOCK);
    fcntl(UrlPipe[1], F_SETFL, O_NONBLOCK);
    int bubu[2];
    pipe(bubu);
    fcntl(bubu[0], F_SETFL, O_NONBLOCK);
    fcntl(bubu[1], F_SETFL, O_NONBLOCK);
    
    
    int AnswerProcess = fork();
    if(AnswerProcess == 0) {
        
        for(int i = 0; ;++i) {
            char* HTML = malloc(1 << 29);
            char* readSymbol = malloc(20);
            char* NewURL = (char*) malloc(1024);
            
            if(read(fd, NewURL, 1024) <= 0)
                break;
            
            char* OurQuery = (char*) malloc(1024);
            strcpy(OurQuery, "curl ");
            strcat(OurQuery, NewURL);
            char* MyUrl[] = {"/bin/bash", "-c", OurQuery, NULL};
            
            int ErOrNot = fork();
            if(ErOrNot < 0) {
                printf("ERROR");
                abort();
            }
            
            else if(ErOrNot == 0) {
                dup2(NULL, STDERR_FILENO);
                int l = dup2(UrlPipe[1], STDOUT_FILENO);
                if(l < 0) {
                    printf("bug");
                    abort();
                }
                execv(MyUrl[0], MyUrl);
                exit(1);
            }
            
            else {
                memset(readSymbol, 0, 20);
                int bsize;
                for(int j = 0; ; ++j) {
                    if((bsize = read(UrlPipe[0], readSymbol, 1)) <= 0)
                        break;
                    strcat(HTML, readSymbol);
                }
                printf("%s\n", HTML);
            
                FindReference(HTML, bubu[1]);
                
            }
            
        }
           

    }
    else  {
//        while (wait(0) > 0);
        int bsize;
        char* readSymbol = malloc(1);
        char *bubu1 = malloc(1 << 20);
        for(int j = 0; ; ++j) {
            if((bsize = read(bubu[0], readSymbol, 1)) <= 0)
                break;
            strcat(bubu1, readSymbol);
        }
        
        printf(bubu1);

    }
    close(UrlPipe[1]);
    close(UrlPipe[0]);
    close(bubu[1]);
    close(bubu[0]);
    unlink(FIFO_Name);
    return 0;
}
