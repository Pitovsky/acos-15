#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct Table{
	size_t size;
	struct List** lists;
};

struct Table* New_Table(size_t size){
	struct Table* new_table = (struct Table*)malloc(sizeof(struct Table));
	new_table->lists = (struct List**)malloc(sizeof(struct List*)*size);
	new_table->size = size;
	size_t i;
	for (i = 0; i < size; ++i){
		new_table->lists[i] = New_List();
	}
	return new_table;
} 

void AddToTable(struct Table* table, char* key, int value, int (*hashfunc)(char* key, int border)){
	AddToList(table->lists[hashfunc(key, table->size)], value, key);
	return;
}

void DeleteFromTable(struct Table* table, char* key, int (*hashfunc)(char* key, int border)){
	DeleteFromList(table->lists[hashfunc(key, table->size)], key);
	return;
}

int GetFromTable(struct Table* table, char* key, int* value, int (*hashfunc)(char* key, int border)){
	return GetFromList(table->lists[hashfunc(key, table->size)], value, key);
}

void DeleteTable(struct Table* table){
	size_t i, size = table->size;
	for(i = 0; i < size; ++i){
		Delete_List(table->lists[i]);
	}
	free(table);
	return;
}
