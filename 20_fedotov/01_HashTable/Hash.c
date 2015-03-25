//
//  Hash.c
//  ACOS_HashTable
//
//  Created by Simon Fedotov on 26.02.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Hash.h"


struct Table* CreateTable(int size) {
    struct Table* NewTable = (struct Table*) malloc(size * sizeof(struct Table));
    NewTable->size = size;
    NewTable -> NumberOfElements = 0;
    NewTable->OurTable = (struct listNode**) malloc(size * sizeof(struct listNode*));
    for(int i = 0; i < size; ++i) {
        NewTable -> OurTable[i] = NULL;
    }
    return NewTable;
}

int Hash(char* key, int size) {
    int result = 0;
    for(int i = 0; i < strlen(key); ++i) {
        result += (int)(key[i]);
    };
    result += key[0] + key[strlen(key) - 1];
    return result % size;
}

struct Table* insertToTable(struct Table* DatTable, struct Data DatData) {
    ++(DatTable -> NumberOfElements);
    struct listNode* CheckList = NULL;
    if(CheckList != NULL) {
        CheckList -> OurData = DatData;
        return DatTable;
    }
    if(DatTable -> OurTable[Hash(DatData . key, DatTable -> size)] == NULL) {
        struct listNode* NewElement = (struct listNode*)malloc(sizeof(struct listNode));
        NewElement -> OurData = DatData;
        NewElement -> nextNode = NULL;
        DatTable -> OurTable[Hash(DatData . key, DatTable -> size)] = NewElement;
    }
    else {
        struct listNode* NewElement = (struct listNode*)malloc(sizeof(struct listNode));
        NewElement -> OurData.data = DatData . data;
        NewElement -> OurData.key = DatData . key;
        NewElement -> nextNode = DatTable -> OurTable[Hash(DatData . key, DatTable -> size)];
        DatTable -> OurTable[Hash(DatData . key, DatTable -> size)] = NewElement;
    }
    if(DatTable -> NumberOfElements >= (4.0 * DatTable -> size) / 3.0) {
        struct Table* DatTableNew = CreateTable((DatTable -> size )* 2);
        DatTableNew = MoveToTable(DatTable, DatTableNew);
        DatTable = DatTableNew;
        return DatTable;
    }
    return DatTable;
}

unsigned short Contains(struct Table* DatTable, struct Data* DatData) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatData -> key, DatTable -> size)];
    //    if(OurPosition -> OurData.key != DatData -> key || (OurPosition ->OurData.key == DatData ->key && OurPosition -> OurData.data != DatData ->data)) {
    //        return 0;
    //    }
    while (OurPosition != NULL) {
        if(!strcmp(OurPosition ->OurData.key , DatData ->key))
            return 1;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return 0;
}

void PrintList(struct listNode* dat) {
    while (dat != NULL) {
        printf("%d\n", dat -> OurData.data);
        dat = dat -> nextNode;
    }
}

struct listNode* Search(struct Table* DatTable, struct Data* DatData) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatData -> key, DatTable -> size)];
    while (OurPosition != NULL) {
        if(!strcmp(OurPosition ->OurData.key , DatData ->key)  && OurPosition -> OurData.data == DatData ->data)
            return OurPosition;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return NULL;
}

struct listNode* SearchPar(struct Table* DatTable, struct Data* DatData) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatData -> key, DatTable -> size)];
    if(OurPosition == NULL)
        return NULL;
    if(OurPosition -> OurData.key == DatData -> key && OurPosition -> OurData.data == DatData -> data) {
        return NULL;
    }
    while ((OurPosition -> nextNode) != NULL) {
        if(!strcmp(OurPosition -> nextNode ->OurData.key , DatData ->key)  && OurPosition -> nextNode -> OurData.data == DatData ->data)
            return OurPosition;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return NULL;
}

unsigned short int Delete(struct Table* DatTable, struct Data* DatData) {
    struct listNode* ToDelete = (struct listNode*)malloc(sizeof(struct listNode*));
    struct listNode* ParentDel = (struct listNode*)malloc(sizeof(struct listNode*));
    struct listNode* tmp = (struct listNode*)malloc(sizeof(struct listNode*));
    ToDelete = Search(DatTable, DatData);
    if (ToDelete == NULL) {
        return 0;
    }
    ParentDel = SearchPar(DatTable, DatData);
    tmp = ToDelete -> nextNode;
    --(DatTable -> NumberOfElements);
    if(ParentDel == NULL) {
        DatTable -> OurTable[Hash(DatData -> key, DatTable -> size)] = ToDelete -> nextNode;
        free(ToDelete);
        ToDelete = NULL;
        return 1;
    }
    ParentDel -> nextNode = tmp;
    ToDelete = NULL;
    return 1;
}



void Deletion(struct listNode* OurNode) {
    if(OurNode != NULL) {
    struct listNode* Next = OurNode -> nextNode;
        free(OurNode);
        OurNode = NULL;
    Deletion(Next);
    }
}

void DeleteTable(struct Table* ThisTable) {
    for(int i = 0; i < ThisTable -> size; ++i) {
        if(ThisTable -> OurTable[i] != NULL) {
            Deletion(ThisTable -> OurTable[i]);
        }
    }
    ThisTable -> NumberOfElements = 0;
}

struct Table* MoveToTable(struct Table* MyFirst, struct Table* MySecond) {
    for(int i = 0; i < MyFirst -> size; ++i) {
        if(MyFirst -> OurTable[i] != NULL) {
            struct listNode* ToMove = (struct listNode*)malloc(sizeof(struct listNode*));
            ToMove = MyFirst -> OurTable[i];
            while (ToMove != NULL) {
                insertToTable(MySecond, ToMove -> OurData);
                ToMove = ToMove -> nextNode;
            }
            
        }
    }
    DeleteTable(MyFirst);
    return MySecond;
}


