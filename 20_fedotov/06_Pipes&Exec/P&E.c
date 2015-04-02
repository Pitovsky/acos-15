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


void FindReference(char* URL) {
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
            while (Check != '"') {
                Check = URL[counter];
                Cash[counter % 9] = Check;
                ++counter;
            }
            
            
            Check = 0;
            if(Cash[(counter - 6) % 9] == 'h' && Cash[(counter - 5) % 9] == 'r' && Cash[(counter - 4) % 9] == 'e' && Cash[(counter - 3) % 9] =='f') {
                
                char* linkName = malloc(255);
                while (Check != '"' && Check != '#' && Check != '(' && Check != ')' && Check != '{' && Check != '}' && Check != ' ') {
                    
                    Check = URL[counter];
                    if(Check == '"' || Check == '#' || Check == '(' || Check == ')' || Check == '{' || Check == '}' || Check == ' ')
                        break;
                    
                    char* str = malloc(255);
                    str[0] = Check;
                    strcat(linkName, str);
                    ++counter;
                }
                
                if(strlen(linkName) != 0)
                    printf("\n%s\n  ", linkName);
                --counter;
            }
            
        }
        
        ++counter;
    }

}



int main(int argc, const char * argv[]) {
    int URLS_NUMBER;
    scanf("%d", &URLS_NUMBER);
    
    char* FIFO_Name = (char*) malloc(sizeof("URLS_SRC"));
    extern char ** environ;
    setenv("URLS_SRC", "OurURLS", 123);
    strcpy(FIFO_Name, "OurURLS");
    unlink(FIFO_Name);
    
    int res = mkfifo(FIFO_Name, O_RDWR | S_IRWXU | O_NONBLOCK);
    if(res < 0) {
        fprintf(stderr, "Can't make pipe");
        exit(1);
    }
    
    int fd = open(FIFO_Name, O_RDWR | O_CREAT | O_NONBLOCK);
    if (fd < 0) {
        perror("");
        exit(1);
    }
    
    //Записываем в Пайп
    for(int i = 0; i < URLS_NUMBER; ++i) {
        char* URL = (char*) malloc(255);
        scanf("%s", URL);
        int OK = write(fd, URL, 255);
        if(!OK)
            exit(1);
    }
    
    //Создаем Пайп с УРЛами
    int UrlPipe[2];
    pipe(UrlPipe);
    fcntl(UrlPipe[0], F_SETFL, O_NONBLOCK);
    fcntl(UrlPipe[1], F_SETFL, O_NONBLOCK);
    
    
    for(int i = 0; ;++i) {
        char* HTML = malloc(1 << 29);
        char* readSymbol = malloc(20);
        char* NewURL = (char*) malloc(255);
        
        if(read(fd, NewURL, 255) <= 0)
            break;
        
        char* OurQuery = (char*) malloc(255);
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
            while(wait(0) >= 0);
            memset(readSymbol, 0, 20);
            int bsize;
            for(int j = 0; ; ++j) {
                if((bsize = read(UrlPipe[0], readSymbol, 1)) <= 0)
                    break;
                strcat(HTML, readSymbol);
            }
            printf("%s\n", HTML);
            FindReference(HTML);
        }
        

    }
    close(UrlPipe[1]);
    close(UrlPipe[0]);
    unlink(FIFO_Name);
    return 0;
}
