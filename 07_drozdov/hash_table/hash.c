#include "hash.h"

int hash_function(char* key, int n)
{
    int v;
    int s = 0;

    for (v = 0; v < strlen(key); v++)
        s += key[v];

    return s % n;
}

struct HashTable* create_table()
{
    struct HashTable* new_table = (struct HashTable*)malloc(sizeof(struct HashTable));
    new_table->size_ = N;
    new_table->table = (struct List*)malloc(new_table->size_ * sizeof(struct List));
    return new_table;
}

void add_pair(struct HashTable* hashtable, struct Pair pair)
{
    int index = hash_function(pair.key, hashtable->size_);
    add_element(&hashtable->table[index], pair);
}

int find_pair(struct HashTable* hashtable, char* key)
{
    int index = hash_function(key, hashtable->size_);
    struct Node* cur_node = (struct Node*)malloc(sizeof(struct Node));
    cur_node = find_element(&hashtable->table[index], key);
    return cur_node->content.value;
}

void delete_pair(struct HashTable* hashtable, char* key)
{
    int index = hash_function(key, hashtable->size_);
    delete_element(&hashtable->table[index], key);
}

void delete_table(struct HashTable* hashtable)
{
    int i;
    for (i = 0; i < hashtable->size_; i++)
        delete_list(&hashtable->table[i]);
    free(hashtable);
}
