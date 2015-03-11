#pragma once
#define MAX_SIZE 15
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node
{
    struct Node *next;
    char* key;
   // void* value;
} Node;

typedef struct Table
{
    size_t size;
  
    Node**buckets;
} Table;

int hash_func(int size, char* key);
Table* createTable(size_t size);
void clearTable(Table *t);
Node* findNode (char* key, Table* t);
Node* insertNode(char* key, Table* t);
void deleteNode(char* key, Table* t);
