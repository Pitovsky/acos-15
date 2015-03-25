//
//  main.c
//  ACOS_StringNumber
//
//  Created by Simon Fedotov on 12.03.15.
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

int WordCounter(FILE* OurFile) {
    int WordCounter = 0;
    int trueOrFalse = 0;
    int Check;
    while ((Check = getc(OurFile)) > 0) {
        if(!(((Check < 48 && Check != 45) || (Check > 57 && Check < 65) || (Check > 90 && Check < 97 && Check != 95) || Check > 122)) && !trueOrFalse) {
            ++WordCounter;
            trueOrFalse = 1;
        }
        else if(((Check < 48 && Check != 45) || (Check > 57 && Check < 65) || (Check > 90 && Check < 97 && Check != 95) || Check > 122)) {
            trueOrFalse = 0;
        }
    }
    return WordCounter;
}


void Processing( char *DatDir, int Depth, int Restriction)
{
    if(Depth > Restriction) {
        printf("popalsya\n");
        return;
    }
    char OurPath[256];
    DIR *MyDir = NULL;
    struct dirent ThisElem;
    struct dirent *ThisElemPtr = NULL;
    int Result = 0;
    unsigned Counter = 0;
    char PathName[PATH_MAX + 1];
    
    //Проверить, можно ли открыть директорию
    MyDir = opendir(DatDir);
    if( MyDir == NULL ) {
        return;
    }
    Result = readdir_r( MyDir, &ThisElem, &ThisElemPtr );
    while( ThisElemPtr != NULL ) {
        struct stat entryInfo;
        
        if(!(strncmp( ThisElem.d_name, ".", PATH_MAX )) ||
           (!strncmp( ThisElem.d_name, "..", PATH_MAX ))) {
            Result = readdir_r( MyDir, &ThisElem, &ThisElemPtr );
            continue;
        }
        strncpy(PathName, DatDir, PATH_MAX);
        strncat(PathName, "/", PATH_MAX);
        strncat(PathName, ThisElem.d_name, PATH_MAX);
        //Тут в ПасНейме лежит имя файла, который мы обрабатываем
        if( lstat(PathName, &entryInfo )== 0 ) {
            //Статистику приняли успешно
            ++Counter;
            
            if(S_ISDIR(entryInfo.st_mode)) {
                //директория
                printf( "processing %s/\n", PathName );
                Processing( PathName, Depth + 1, Restriction);
            } else if(S_ISREG(entryInfo.st_mode)) {
                //Просто файл, считаем кол-во слов
                FILE* OurFile = fopen(PathName, "rw");
                printf("%s\n", OurPath);
                printf("\nWord number is : %d \n", WordCounter(OurFile));
                printf("\n%s\n", PathName);
                fclose(OurFile);
            }
            else if( S_ISLNK( entryInfo.st_mode ) ) {
                //если Симлинк
                
                char targetName[PATH_MAX + 1];
                if( readlink(PathName, targetName, PATH_MAX ) != -1 ) {
                    printf( "\t%s -> %s\n", PathName, targetName );
                    FILE* OurFile = fopen(targetName, "rw");
                    if(OurFile != NULL) {
                        printf("%s\n", OurPath);
                        printf("\nWord Linked is : %d \n", WordCounter(OurFile));
                        printf("\n%s\n", PathName);
                        fclose(OurFile);
                    }
                } else {
                    printf( "\t%s -> error symlink\n", PathName );
                }
            }
        } else {
            printf( "Error in stat %s: \n", PathName);
        }
        Result = readdir_r( MyDir, &ThisElem, &ThisElemPtr );
    }
    
    //Закрываем каталог и выводим всю нужную инфу
    closedir( MyDir );
}



int main( int argc, char * argv[])
{
    int Position = 0;
    int Restriction = atoi(argv[argc - 1]);
    if(Restriction == 0)
        Restriction = INT32_MAX;
    else if(Restriction == 1)
        Restriction = 0;
    else
        Restriction = 1;
    for( Position = 1; Position < argc; Position++ ) {
        Processing( argv[Position], 0, Restriction);
    }
    
    return 0;
}
