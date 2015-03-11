#include <stdio.h>

typedef struct Node
{
    char *key;
    int value;
    struct Node *next;

}Node;

typedef struct HashTable
{
    size_t size;
    struct Node** array;
}Hashtable;

Hashtable *CreateHashtable(size_t size);
int hash(char* string, int size);
void InsertData(Hashtable *hashtable, char *key, int value);
char *GetData(Hashtable *hashtable, char *key);
void DeleteData(Hashtable *hashtable, char *key);
void DeleteHashtable(Hashtable *hashtable);
