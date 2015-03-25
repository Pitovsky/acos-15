//
//  test2.cpp
//  ACOS_HashTable
//
//  Created by Simon Fedotov on 16.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

//
//  testfile.c
//  ACOS_HashTable
//
//  Created by Simon Fedotov on 27.02.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Hash.h"


int main() {
    FILE *myFile = fopen("/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable/test2.txt", "rb"); //У меня тольkо так возможно
    FILE *myOut =  fopen("/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable/stud_ans.txt", "w");//подключить файлы
    int NewSize;
    fscanf(myFile, "%d", &NewSize);
    struct Table* NewTable = CreateTable(NewSize);
    struct Table* DatTableNew = CreateTable((NewTable -> size )* 2);
    struct Table* MyTable = CreateTable(0);
    MyTable = NewTable;
    for(int i = 0; i < NewSize; ++i) {
        int FirstData;
        char* FirstKey = (char*) malloc(255 * sizeof(char));
        fscanf(myFile, "%s%d", FirstKey, &FirstData);
        struct Data* NewData = (struct Data*)malloc(sizeof(struct Data));
        NewData -> data = FirstData;
        NewData -> key = FirstKey;
        insertToTable(MyTable, *NewData);
        if(MyTable -> NumberOfElements >= (4.0 * MyTable -> size) / 3.0) {
            struct Table* DatTable = CreateTable(NewTable -> size * 2);
            DatTableNew = MoveToTable(NewTable, DatTable);
            MyTable = DatTableNew;
        }
        
    }
    int FirstData = 0;
    char* FirstKey = (char*) malloc(255 * sizeof(char));
    int counter = 0;
    FILE* ToDelete = fopen("/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable/test2.txt", "rb");
    fscanf(ToDelete, "%d", &NewSize);
    while(strcmp(FirstKey, "/////")) {
        struct Data* NewData = (struct Data*)malloc(sizeof(struct Data));
        ++counter;
        if(!strcmp(FirstKey, "/////"))
            break;
        fscanf(ToDelete, "%s%d", FirstKey, &FirstData);
        NewData -> data = FirstData;
        NewData -> key = FirstKey;
        if(counter % 6 == 1) {
            Delete(MyTable, NewData);
        }
    }
    
    struct Data* NewData = (struct Data*)malloc(sizeof(struct Data));
    FILE* ToInput = fopen("/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable/test2.txt", "rb");
    fscanf(ToInput, "%d", &NewSize);
    FirstData = 0;
    FirstKey = (char*) malloc(255 * sizeof(char));
    while(strcmp(FirstKey, "/////")) {
        fscanf(ToInput, "%s%d", FirstKey, &FirstData);
        if(!strcmp(FirstKey, "/////"))
            break;
        NewData -> data = FirstData;
        NewData -> key = FirstKey;
        if(Contains(MyTable, NewData)) {
            fprintf(myOut, "%s\n", "YES");
        }
        else {
            fprintf(myOut, "%s\n", "NO");
        }
    }
    fclose(myFile);
    fclose(myOut);
    DeleteTable(NewTable);
    DeleteTable(DatTableNew);
    printf("Everything is alright!\n");
    return 0;
}