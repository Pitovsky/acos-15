//
//  Hash.h
//  ACOS_HashTable
//
//  Created by Simon Fedotov on 26.02.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//



struct listNode {
    char* OurKey;
    struct listNode* nextNode;
};

struct Table {
    int NumberOfElements;
    int size;
    struct listNode** OurTable;
};



struct Table* CreateTable(int size);
int Hash(char* key, int size);
struct Table* insertToTable(struct Table* DatTable, char* DatKey);
unsigned short Contains(struct Table* DatTable, char* DatKey);
void PrintList(struct listNode* dat);
struct listNode* Search(struct Table* DatTable, char* DatKey);
struct listNode* SearchPar(struct Table* DatTable,char* DatKey);
unsigned short int Delete(struct Table* DatTable,char* DatKey);
void Deletion(struct listNode* OurNode);
void DeleteTable(struct Table* ThisTable);
struct Table* MoveToTable(struct Table* MyFirst, struct Table* MySecond);
