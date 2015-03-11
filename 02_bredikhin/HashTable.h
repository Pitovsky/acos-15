#include <stdio.h>
#include <stdlib.h>

typedef long long hashTableIndex;
typedef char* T;
typedef struct Node_ {
	struct Node_ *next = NULL;
	T string;
} Node;
hashTableIndex hash(T s);
Node** CreateHashTable();
void InsertNode(Node **hashTable, T s);
Node* FindNode(Node **hashTable, T s);
void DeleteNode(Node **hashTable, T s);
void DeleteHashTable(Node **hashTable);