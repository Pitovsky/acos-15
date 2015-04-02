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
#include "NewHash.h"


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

struct Table* insertToTable(struct Table* DatTable, char* DatKey) {
    ++(DatTable -> NumberOfElements);
    if(DatTable -> OurTable[Hash(DatKey, DatTable -> size)] == NULL) {
        struct listNode* NewElement = (struct listNode*)malloc(sizeof(struct listNode));
        NewElement -> OurKey = DatKey;
        NewElement -> nextNode = NULL;
        DatTable -> OurTable[Hash(DatKey, DatTable -> size)] = NewElement;
    }
    else {
        if(Contains(DatTable, DatKey))
            return DatTable;
        struct listNode* NewElement = (struct listNode*)malloc(sizeof(struct listNode));
        NewElement -> OurKey = DatKey;
        NewElement -> nextNode = DatTable -> OurTable[Hash(DatKey, DatTable -> size)];
        DatTable -> OurTable[Hash(DatKey, DatTable -> size)] = NewElement;
    }
    if(DatTable -> NumberOfElements >= (4.0 * DatTable -> size) / 3.0) {
        struct Table* DatTableNew = CreateTable((DatTable -> size )* 2);
        DatTableNew = MoveToTable(DatTable, DatTableNew);
        DatTable = DatTableNew;
        return DatTable;
    }
    return DatTable;
}

unsigned short Contains(struct Table* DatTable, char* DatKey) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatKey, DatTable -> size)];
    //    if(OurPosition -> OurData.key != DatData -> key || (OurPosition ->OurData.key == DatData ->key && OurPosition -> OurData.data != DatData ->data)) {
    //        return 0;
    //    }
    while (OurPosition != NULL) {
        if(!strcmp(OurPosition -> OurKey, DatKey))
            return 1;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return 0;
}


struct listNode* Search(struct Table* DatTable, char* DatKey) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatKey, DatTable -> size)];
    while (OurPosition != NULL) {
        if(!strcmp(OurPosition -> OurKey , DatKey)  && OurPosition -> OurKey == DatKey)
            return OurPosition;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return NULL;
}

struct listNode* SearchPar(struct Table* DatTable, char* DatKey) {
    struct listNode* OurPosition = (struct listNode*)malloc(sizeof(struct listNode*));
    OurPosition = DatTable -> OurTable[Hash(DatKey, DatTable -> size)];
    if(OurPosition == NULL)
        return NULL;
    if(OurPosition -> OurKey == DatKey) {
        return NULL;
    }
    while ((OurPosition -> nextNode) != NULL) {
        if(!strcmp(OurPosition -> nextNode -> OurKey, DatKey)  && OurPosition -> nextNode -> OurKey == DatKey)
            return OurPosition;
        else
            OurPosition = OurPosition -> nextNode;
    }
    return NULL;
}

unsigned short int Delete(struct Table* DatTable, char* DatKey) {
    struct listNode* ToDelete = (struct listNode*)malloc(sizeof(struct listNode*));
    struct listNode* ParentDel = (struct listNode*)malloc(sizeof(struct listNode*));
    struct listNode* tmp = (struct listNode*)malloc(sizeof(struct listNode*));
    ToDelete = Search(DatTable, DatKey);
    if (ToDelete == NULL) {
        return 0;
    }
    ParentDel = SearchPar(DatTable, DatKey);
    tmp = ToDelete -> nextNode;
    --(DatTable -> NumberOfElements);
    if(ParentDel == NULL) {
        DatTable -> OurTable[Hash(DatKey, DatTable -> size)] = ToDelete -> nextNode;
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
                insertToTable(MySecond, ToMove -> OurKey);
                ToMove = ToMove -> nextNode;
            }
            
        }
    }
    DeleteTable(MyFirst);
    return MySecond;
}


