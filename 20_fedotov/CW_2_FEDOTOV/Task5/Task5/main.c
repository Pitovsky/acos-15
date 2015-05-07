//
//  main.c
//  Task5
//
//  Created by Simon Fedotov on 07.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <pthread/pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

struct Info{
    pthread_mutex_t *DatMutex;
    int Number;
};


int Co_Counter(const char *DatName){
    DIR *OurDir;
    struct dirent *DatDir;
    
    if (!(OurDir = opendir(DatName))){
        perror("Error While Opening Dir");
        exit(1);
    }
    
    if (!(DatDir = readdir(OurDir))){
        perror("Error While REading Dir");
        exit(1);
    }
    
    int Quantity = 0;
    
    do{
        char DatPath[1 << 10];
        
        int Size = snprintf(DatPath, sizeof(DatPath) - 1, "%s/%s", DatName, DatDir->d_name);
        DatPath[Size] = 0;
        
        if (DatDir -> d_type == DT_DIR){
            if (strcmp(DatDir -> d_name, "..") == 0 || strcmp(DatDir->d_name, ".") == 0)
                continue;
            Quantity += Co_Counter(DatPath);
        }
        else if (DatDir->d_type == DT_LNK || DatDir->d_type == DT_REG){
            if (strncmp(DatDir->d_name, ".", 1) == 0)
                continue;
            
            Quantity++;
        }
    }
    while ((DatDir = readdir(OurDir)));
    closedir(OurDir);
    
    return Quantity;
}

int Cs_Counter(const char *DatName){
    DIR *OurDir;
    struct dirent *DatDir;
    
    if (!(OurDir = opendir(DatName))){
        perror("Error While Opening Dir");
        exit(1);
    }
    
    if (!(DatDir = readdir(OurDir))){
        perror("Error While REading Dir");
        exit(1);
    }
    
    int Length = 0;
    
    do{
        char DatPath[1 << 10];
        
        int Size = snprintf(DatPath, sizeof(DatPath) - 1, "%s/%s", DatName, DatDir->d_name);
        DatPath[Size] = 0;
        
        if (DatDir->d_type == DT_DIR){
            if (strcmp(DatDir->d_name, ".") == 0 || strcmp(DatDir->d_name, "..") == 0)
                continue;
            Length += Cs_Counter(DatPath);
        }
        else if (DatDir->d_type == DT_LNK || DatDir->d_type == DT_REG){
            if (strncmp(DatDir->d_name, ".", 1) == 0)
                continue;
            
            FILE *fd;
            if ((fd = fopen(DatPath, "rw")) == NULL){
                perror("Error While opening file");
                exit(1);
            }
            
            fseek(fd, 0, SEEK_END);
            Length += ftell(fd);
            
            fclose(fd);
        }
    } while ((DatDir = readdir(OurDir)));
    closedir(OurDir);
    
    return Length;
}


void *ToJob(void *Value){
    struct Info *info = Value;
    
    while (1){
        pthread_mutex_lock(info -> DatMutex);
        
        printf("Dat Command is: ");
        
        int Command = -1;
        char FilePath[128] = {0};
        
        char comd[30] = {0};
        
        if (scanf("%s %s", comd, FilePath) == EOF){
            pthread_mutex_unlock(info -> DatMutex);
            return NULL;
        }
        
        if (strcmp("co", comd) == 0)
            Command = 1;
        
        else
            Command = 0;
        
        
        pthread_mutex_unlock(info -> DatMutex);
        
        int Res = -1;
        if (Command == 0)
            Res = Cs_Counter(FilePath);
        
        else if (Command == 1)
            Res = Co_Counter(FilePath);
        
        
        printf("%s %s %d\n", Command ? "co" : "cs", FilePath, Res);
    }
}

int main(int argc, const char * argv[]){
    pthread_mutex_t OurMutex;
    pthread_mutex_init(&OurMutex, NULL);
    
    int ThreadsNumber = atoi(argv[1]);
    
    pthread_t **Threads = (pthread_t **)calloc(ThreadsNumber, sizeof(pthread_t *));
    
    struct Info *info = (struct Info *)malloc(sizeof(struct Info));
    info -> DatMutex = &OurMutex;
    
    for (int i = 0; i < ThreadsNumber; i++){
        Threads[i] = (pthread_t *)calloc(1, sizeof(pthread_t));
        pthread_create(Threads[i], NULL, ToJob, info);
    }
    
    for (int i = 0; i < ThreadsNumber; i++){
        if(pthread_join(*Threads[i], NULL)){
            perror("Error while joining threads");
            exit(1);
        }
        free(Threads[i]);
    }
    
    return 0;
}
