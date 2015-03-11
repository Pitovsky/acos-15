#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

typedef struct Table {
    Node** cells; // array with chains
    size_t size;
    size_t filled;
} Table;

void  node_create(Node** node, const char* key);
void  node_delete(Node* node);

void  table_create(Table** tbl, size_t size);
void  table_delete(Table* tbl);
void  table_clear(Table* tbl);

//Node* table_search(Table* tbl, char* key);
void  table_insert(Table** tbl, const char* key, const int value);
int   table_get(Table** tbl, const char* key);
void  table_erase(Table** tbl, const char* key);
