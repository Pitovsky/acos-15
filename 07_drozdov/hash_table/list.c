#include "list.h"

struct List* create_list()
{
    struct List* new_list = (struct List*)malloc(sizeof(struct List));
    return new_list;
}

void add_element(struct List* list, struct Pair a)
{
    if (list->head == NULL)
    {
        list->head = (struct Node*)malloc(sizeof(struct Node));
        list->tail = (struct Node*)malloc(sizeof(struct Node));
        list->head->content = a;
        list->head->next = list->head->prev = NULL;
        list->tail = list->head;
        return;
    }

    struct Node* cur_node = (struct Node*)malloc(sizeof(struct Node));
    cur_node->content = a;
    list->tail->next = cur_node;
    cur_node->prev = list->tail;
    cur_node->next = NULL;
    list->tail = cur_node;
    free(cur_node);
}

struct Node* find_element(struct List* list, char* key)
{
    if (list->head == NULL)
        return -1;

    struct Node* cur_node = (struct Node*)malloc(sizeof(struct Node));
    cur_node = list->head;
    while ((strcmp(cur_node->content.key, key) != 0) && (cur_node != NULL))
        cur_node = cur_node->next;

    if (cur_node == NULL)
        return -1;
    return cur_node;
}

void delete_element(struct List* list, char* key)
{
    struct Node* cur_node = (struct Node*)malloc(sizeof(struct Node));
    cur_node = find_element(list, key);

    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    free(cur_node);
}

void delete_list(struct List* list)
{
    struct Node cur_node;
    cur_node = *list->head->next;
    while (cur_node.next != list->tail)
    {
        free(cur_node.prev);
        cur_node = *cur_node.next;
    }
    free(list);
}
