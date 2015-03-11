#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

struct Pair
{
    char *key;
    int value;
};

struct Node
{
    struct Node* next;
    struct Node* prev;
    struct Pair content;
};

struct List
{
    struct Node* head;
    struct Node* tail;
};

struct List* create_list();
void add_element(struct List*, struct Pair);
struct Node* find_element(struct List*, char*);
void delete_element(struct List*, char*);
void delete_list(struct List*);

#endif // LIST_H_INCLUDED
