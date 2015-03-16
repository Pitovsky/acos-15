#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#define $$ fprintf(stderr, "%i\n", __LINE__);

typedef struct List_t
{
    char* key;
    void* data;
    struct List_t* next;
} List_t;


void killList(List_t* list)
{
    if (list)
    {
        free(list->key);
        killList(list->next);
    }
    free(list);
}

typedef uint64_t Hash_t;
static const Hash_t HASH_BASE = 257, HASH_MOD = (1ull << 61) + 15ull;

typedef struct HashTable_t
{
    List_t **bins;
    size_t size;
    size_t nKeys;
} HashTable_t;

Hash_t hashString(const char* string)
{
    Hash_t hash = 1;

    while (*string)
        hash = (hash*HASH_BASE + ((unsigned char)*(string++))) % HASH_MOD;

    return hash;
}

HashTable_t* createHashTable(size_t nBins)
{
    HashTable_t* table = malloc(sizeof(HashTable_t));
    table->bins = malloc(sizeof(List_t**) * nBins);
    table->size = nBins;
    table->nKeys = 0;

    memset(table->bins, 0, nBins * sizeof(List_t**));

    return table;
}

void killHashTable(HashTable_t* table)
{
    for (size_t i = 0; i < table->size; i++)
        killList(table->bins[i]);
    free(table->bins);
    free(table);
}

void insertToHashTable(HashTable_t* table, const char* key, void* value)
{
    Hash_t hash = hashString(key) % table->size;

    List_t* newNode = malloc(sizeof(List_t));

    newNode->next = table->bins[hash];
    newNode->data = value;
    newNode->key = strdup(key);
    table->bins[hash] = newNode;

    table->nKeys++;
}

void* findInHashTable(HashTable_t* table, const char* key)
{
    Hash_t hash = hashString(key) % table->size;
    for (List_t* i = table->bins[hash]; i; i = i->next)
        if (strcmp(key, i->key) == 0)
            return i->data;
    return 0;
}

void eraseFromHashTable(HashTable_t* table, const char* key)
{
    Hash_t hash = hashString(key) % table->size;
    if (strcmp(table->bins[hash]->key, key) == 0)
    {
        List_t* p = table->bins[hash];
        table->bins[hash] = table->bins[hash]->next;
        free(p->key);
        free(p);
        return;
    }

    for (List_t *prev = table->bins[hash], *curr = table->bins[hash]->next;
         curr;
         prev = curr, curr = curr->next)
    {
        if (strcmp(key, curr->key) == 0)
        {
            prev->next = curr->next;
            free(curr->key);
            free(curr);
            break;
        }
    }}

#define v(x) ((void*)(size_t)(x))

int main()
{
    HashTable_t* table = createHashTable(2000000);

    insertToHashTable(table, "Stepan", v(15));
    eraseFromHashTable(table, "Stepan");
    srand(2);
    for (int i = 0; i < 100000; i++)
    {
        char string[10] = "";
        for (int k = 0; k < 9; k++)
            string[k] = (char)(rand() & 0xff);
        insertToHashTable(table, string, v(i + 42));
    }

    srand(2);
    for (int i = 0; i < 100000; i++)
    {
        char string[10] = "";
        for (int k = 0; k < 9; k++)
            string[k] = (char)(rand() & 0xff);
        eraseFromHashTable(table, string);
    }

    killHashTable(table);
}
