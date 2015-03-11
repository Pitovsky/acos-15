#include <stdio.h>
#include "HashTable.h"
#include <math.h>
#include <string.h>

Hashtable *CreateHashtable(size_t size){
    Hashtable* hashtable = (Hashtable*)malloc(size * sizeof(Hashtable*));
    hashtable->size = size;
    hashtable->array = (Node**)malloc(size * sizeof(Node*));

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

void InsertData(Hashtable *hashtable, char *key, int value){
    char *NewString = (char*)malloc(strlen(key)*sizeof(char));
    int i;
    for(i = 0; i < strlen(key); i++){
        NewString[i] = key[i];
        if(hashtable->array[hash(key, hashtable->size)] == NULL){
            hashtable->array[hash(key, hashtable->size)] == (Node*)malloc(1*sizeof(Node));
            hashtable->array[hash(key, hashtable->size)]->next = NULL;
            hashtable->array[hash(key, hashtable->size)]->value = value;
            hashtable->array[hash(key, hashtable->size)]->key = NewString;
        }
        else{
            Node *NextNode = hashtable->array[hash(key, hashtable->size)];
            while(NextNode->next != NULL){
                NextNode = NextNode->next;
            }
            NextNode->next = (Node*)malloc(1*sizeof(Node));
            NextNode->next->next = NULL;
            NextNode->next->key = NewString;
            NextNode->next->value = value;
        }
    }
}

char *GetData(Hashtable *hashtable, char *key){
    Node *NewNode = hashtable->array[hash(key, hashtable->size)];
    while(NewNode != NULL && strcmp(NewNode->key, key) != 0)
        NewNode = NewNode->next;
    if(NewNode == NULL)
        return "";
    return NewNode -> key;
}

void DeleteData(Hashtable *hashtable, char *key){
    Node *NewNode = hashtable->array[hash(key, hashtable->size)];
    if(hashtable->array[hash(key, hashtable->size)] == NULL)
        return;
    if(hashtable->array[hash(key, hashtable->size)]->next == NULL){
        free(hashtable->array[hash(key, hashtable->size)]);
        hashtable->array[hash(key, hashtable->size)] = NULL;
        return;
    }
    if(strcmp(NewNode->key, key) == 0){
        Node *ClearNode = NewNode->next;
        free(NewNode->key);
        hashtable->array[hash(key, hashtable->size)] = ClearNode;
        return;
    }
    while(NewNode->next != NULL && strcmp(NewNode->next->key, key) !=0){
        NewNode = NewNode->next;
    }
    Node *ClearNode = NewNode->next->next;
    free(NewNode->next->key);
    free(NewNode->next);
    NewNode->next = ClearNode;
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




