#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HT_H


typedef char* key;

typedef struct data
{
	key KEY;
	int my_data;
} data;
typedef struct node
{
	struct node* next;
	struct node* previous;
	struct data* input;
} node;
typedef struct table
{
	int Size;
	int NumberOfElements;
	struct node** mytable;
} table;

int HashU(char* s, int M);
table* CreateTable(int tsize);
int InsertInTheTable(table* Table, data* Data);
data* FindInTheTable(table* Table, key KEY);
int DeleteFromTable(table* Table, key KEY);
void PrintTable(table* Table);
void PrintList(struct node* list);
int HashU(char* s, int M);
void ClearTable(table* Table);
void DeleteTable(table* Table);
int hashtest();

#include "HT.hpp"