#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "list.h"
#define N 10

struct HashTable
{
    struct List* table;
    int size_;
};

struct HashTable* create_table();
void add_pair(struct HashTable*, struct Pair);
int find_pair(struct HashTable*, char*);
void delete_pair(struct HashTable*, char*);
void delete_table(struct HashTable*);

#endif // HASH_H_INCLUDED
