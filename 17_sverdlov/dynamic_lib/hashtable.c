#include "hashtable.h"

#define MOD 1000000007

int get_string_hash(const char* s) {
    int res = 0, f = 29, i = 0;
    for (; s[i]; ++i) {
        res = (res * 1ll * f + (int)(s[i] - '0' + 1)) % MOD;
    }
    return res;
}

// node constructor
void node_create(Node** node, const char* key) {
    (*node) = (Node*) malloc(sizeof(Node));
    (*node)->next = NULL;
    (*node)->key = strdup(key);
    (*node)->value = 0;
}

void node_delete_single(Node* node) {
    if (!node) {
        return;
    }
    free(node->key);
    free(node->next);
    free(node);
}

// node destructor
void node_delete(Node* node) {
    if (!node) {
        return;
    }
    if (node->next) {
        node_delete(node->next);
    }
    node_delete_single(node);
}

// table constructor
void table_create(Table** tbl, size_t size) {
    (*tbl) = (Table*)malloc(sizeof(Table));
    (*tbl)->size = size;
    (*tbl)->cells = (Node**) calloc(size, sizeof(Node*)); // want to initialize pointers to NULL's (0)
    (*tbl)->filled = 0;
}

// table clear
void table_clear(Table* tbl) {
    size_t i = 0;
    for (; i < tbl->size; ++i) {
        node_delete(tbl->cells[i]);
    }
    tbl->filled = 0;
}

// table destructor
void table_delete(Table* tbl) {
    table_clear(tbl);
    free(tbl->cells);
    free(tbl);
}


// table reallocator
void table_expand_twice(Table** tbl) {
    size_t i;
    size_t size = (*tbl)->size;
    Table* new_tbl;
    table_create(&new_tbl, size * 2);
    for (i = 0; i < (*tbl)->size; ++i) {
        Node* node = (*tbl)->cells[i];
        while (node != NULL) {
            table_insert(&new_tbl, node->key, node->value);
            node = node->next;
        }
    }
    table_delete((*tbl));
    (*tbl) = new_tbl;
}

size_t table_position_by_key(Table* tbl, const char* key) {
    // calculate hash
    size_t hash = get_string_hash(key);
    return hash % tbl->size;
}

Node* table_search(Table* tbl, const char* key) {
    size_t pos = table_position_by_key(tbl, key);

    // search for node with same key
    Node** node = &tbl->cells[pos];
    if (!(*node)) {
        node_create(node, key);
        ++tbl->filled;
    }
    while ((*node)->next && strcmp(key, (*node)->key) != 0) {
        (*node) = (*node)->next;
    }
    if (strcmp((*node)->key, key) != 0) {
        node_create(&(*node)->next, key);
        (*node) = (*node)->next;
        ++tbl->filled;
    }
    assert(strcmp((*node)->key, key) == 0);
    return *node;
}

int table_full(Table* tbl)
{
    return tbl->filled * 3 > tbl->size * 4;
}

void table_insert(Table** tbl, const char* key, const int value) {
    Node* node = table_search(*tbl, key);
    node->key = strdup(key);
    node->value = value;
    if (table_full(*tbl)) {
        table_expand_twice(tbl);
    }
}

int table_get(Table** tbl, const char* key) {
    Node* node = table_search(*tbl, key);
    if (table_full(*tbl)) {
        table_expand_twice(tbl);
        node = table_search(*tbl, key);
    }
    return node->value;
}

void table_erase(Table** tbl, const char* key) {
    size_t pos = table_position_by_key(*tbl, key);

    Node* node = (*tbl)->cells[pos];
    if (!node) {
        return;
    }
    while (node->next) {
        if (strcmp(node->next->key, key) == 0) {
            // swap-like erase
            Node* nn_node = node->next->next;
            node_delete_single(node->next);
            node->next = nn_node;
            --(*tbl)->filled;
            return;
        }
        node = node->next;
    }
    // case of only 1 node in chain
    if (strcmp(node->key, key) == 0) {
        node_delete_single(node);
        (*tbl)->cells[pos] = NULL;
        --(*tbl)->filled;
    }
}
