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
#include "NewHash.c"

int WordCounter(FILE* OurFile) {
    int WordCounter = 0;
    int trueOrFalse = 0;
    int Check;
    while ((Check = getc(OurFile)) > 0) {
        if(!(((Check < '0' && Check != '-') || (Check > '9' && Check < 'A') || (Check > 'z' && Check < 'a' && Check != '_') || Check > 'z')) && !trueOrFalse) {
            ++WordCounter;
            trueOrFalse = 1;
        }
        else if(((Check < '0' && Check != '-') || (Check > '9' && Check < 'A') || (Check > 'z' && Check < 'a' && Check != '_') || Check > 'z')) {
            trueOrFalse = 0;
        }
    }
    return WordCounter;
}


void Processing( char *DatDir, int Depth, int Restriction, int Flag, struct Table* DatTable)
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
    char FileName[PATH_MAX + 1];
    
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
        strncpy(FileName, DatDir, PATH_MAX);
        strncat(FileName, "/", PATH_MAX);
        strncat(FileName, ThisElem.d_name, PATH_MAX);
        //Тут в ПасНейме лежит имя файла, который мы обрабатываем
        if( lstat(FileName, &entryInfo )== 0 ) {
            //Статистику приняли успешно
            ++Counter;
            
            if(S_ISDIR(entryInfo.st_mode)) {
                
                //директория
                printf( "processing %s/\n", FileName );
                Processing( FileName, Depth + 1, Restriction, Flag, DatTable);
            } else if(S_ISREG(entryInfo.st_mode) && !Contains(DatTable, FileName)) {
                
                DatTable = insertToTable(DatTable, FileName);
         

                
                //Просто файл, считаем кол-во слов
                FILE* OurFile = fopen(FileName, "rw");
                printf("\nWord number is : %d - ", WordCounter(OurFile));
                printf("%s\n", FileName);
                fclose(OurFile);
            }
            else if( S_ISLNK( entryInfo.st_mode ) && Flag) {
                //если Симлинк
                
               DatTable = insertToTable(DatTable, FileName);
      

                char targetName[PATH_MAX + 1];
                if( readlink(FileName, targetName, PATH_MAX ) != -1 && !Contains(DatTable, targetName)) {
                    
                    
                  DatTable = insertToTable(DatTable, targetName);
                 

                    
                    printf( "\n\t%s -> %s", FileName, targetName );
                    FILE* OurFile = fopen(targetName, "rw");
                    if(OurFile != NULL) {
                        printf("\nWord Linked is : %d -", WordCounter(OurFile));
                        printf("%s\n", targetName);
                        fclose(OurFile);
                    }
                } else {
                    printf( "\t%s -> error symlink\n", FileName );
                }
            }
        } else {
            printf( "Error in stat %s: \n", FileName);
        }
        Result = readdir_r( MyDir, &ThisElem, &ThisElemPtr );
    }
    
    //Закрываем каталог и выводим всю нужную инфу
    closedir( MyDir );
}



int main( int argc, char * argv[])
{
    struct Table* OurTable = CreateTable(5);
    int counter = 1;
    int result = 0;
    while (counter < argc && !result) {
        if(!strcmp(argv[counter], "-s")){
            result = 1;
            break;
        }
        ++counter;
    }
    printf("%d", result);
    int Position = 0;
    int Restriction = atoi(argv[argc - 1]);
    if(Restriction == 0)
        Restriction = INT32_MAX;
    else if(Restriction == 1)
        Restriction = 0;
    else
        Restriction = 1;
    
    for(Position = 1; Position < argc; ++Position) {
        Processing(argv[Position], 0, Restriction, result, OurTable);
    }
    return 0;
}
