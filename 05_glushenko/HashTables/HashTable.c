#include <stdio.h>
#include "HashTable.h"
#include <math.h>
#include <string.h>

Hashtable *CreateHashtable(size_t size){
    Hashtable* hashtable = (Hashtable*)malloc(size * sizeof(Hashtable*));
    hashtable->size = size;
    hashtable->array = (Node**)malloc(size * sizeof(Node*));
    hashtable->NumberOfElements = 0;
    int i;
    for (i = 0; i < size; ++i)
    {
        hashtable->array[i] = NULL;
    }
    return hashtable;
}

int hash(char* key, int size)
{
    int i = 0, sum = 0;
    for (i = 0; i < strlen(key); ++i)
        sum = (sum * 257 + (int)key[i])%(10^9 + 7);
    return sum % size;
}

void InsertData(Hashtable *hashtable, char *key,const  void* value){
    char *NewString = (char*)malloc(strlen(key)*sizeof(char));
    int i;
    int CurHash = hash(key, hashtable->size);
    hashtable->NumberOfElements++;
    int n = strlen(key);
    for(i = 0; i < n; i++){
        NewString[i] = key[i];
        if(hashtable->array[CurHash] == NULL){
            hashtable->array[CurHash] == (Node*)malloc(1*sizeof(Node));
            hashtable->array[CurHash]->next = NULL;
            hashtable->array[CurHash]->value = value;
            hashtable->array[CurHash]->key = NewString;
        }
        else{
            Node *NextNode = hashtable->array[CurHash];
            while(NextNode->next != NULL){
                NextNode = NextNode->next;
            }
            NextNode->next = (Node*)malloc(1*sizeof(Node));
            NextNode->next->next = NULL;
            NextNode->next->key = NewString;
            NextNode->next->value = value;
        }
    }
    ExpandTable(hashtable);
}

void *GetData(Hashtable *hashtable, char *key){
    Node *NewNode = hashtable->array[hash(key, hashtable->size)];
    while(NewNode != NULL && strcmp(NewNode->key, key) != 0)
        NewNode = NewNode->next;
    if(NewNode == NULL)
        return NULL;
    return NewNode -> value;
}

void DeleteData(Hashtable *hashtable, char *key){
    int CurHash = hash(key, hashtable->size);
    Node *NewNode = hashtable->array[CurHash];
    if(hashtable->array[CurHash] == NULL)
        return;
    if(hashtable->array[CurHash]->next == NULL){
        free(hashtable->array[CurHash]);
	hashtable->NumberOfElements--;
        hashtable->array[CurHash] = NULL;
        return;
    }
    if(strcmp(NewNode->key, key) == 0){
        Node *ClearNode = NewNode->next;
        free(NewNode->key);
	hashtable->NumberOfElements--;
        hashtable->array[CurHash] = ClearNode;
        return;
    }
    while(NewNode->next != NULL && strcmp(NewNode->next->key, key) !=0){
        NewNode = NewNode->next;
    }
    Node *ClearNode = NewNode->next->next;
    free(NewNode->next->key);
    free(NewNode->next);
    NewNode->next = ClearNode;
    hashtable->NumberOfElements--;
}

void DeleteHashtable(Hashtable *hashtable){
    int i;
    for(i = 0; i < hashtable->size; i++){
        if(hashtable->array[i] != NULL){
            Node *NewNode = hashtable->array[i];
            while(NewNode != NULL){
                free(NewNode->key);
                NewNode = NewNode->next;
            }
        }
        free(hashtable->array[i]);
    }
    free(hashtable->array);
}

void Copy(Hashtable* hashtable, Hashtable* new){
	if(hashtable == NULL || new == NULL)
		return;
	unsigned int i;
	for(i = 0; i < hashtable->size; i++){
		Node* CurNode = new->array[i];
		while(CurNode != NULL){
			InsertData(hashtable, CurNode->key, CurNode->value);
			CurNode = CurNode->next;
		}	
	}
}	

Hashtable *ExpandTable(Hashtable *hashtable){
	if(hashtable->NumberOfElements >= 4/3 * hashtable->size){
		Hashtable *NewTable = CreateHashtable(hashtable->size*2);
		Copy(hashtable, NewTable);
		printf("Table expanded. New size: %d\n", NewTable->size);
		return NewTable;
	}
	else
		return hashtable;
}



