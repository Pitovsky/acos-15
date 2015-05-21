#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

typedef struct node{
    char* key;
    void* data;
    struct node* next;
} node;

node** create(int M){
    node** T = (node**)malloc(M*sizeof(node*));
    return T;
}

unsigned int hash(char* key, int M){
    return (int)key%M;
}

void insert(void* data,char *key, node** T, int M)
{
        int index = hash(key, M);
        T[index] = (node*)malloc(sizeof(node));
        if (T[index] != NULL) {
                T[index]->next = NULL;
                T[index]->key = key;
		T[index]->data = data;
        }
}
node* search_by_key(char *key, node** T, int M){
    int index = hash(key, M);
    while (T[index]) {
                if (T[index]->key == key) return T[index];
                T[index] = T[index]->next;
        }
        perror("can't find element");
				exit(1);
}

void delete(char* key, node **T, int M)
{
        int index = hash(key, M);
        node* N = search_by_key(key, T, M);
				if (N = NULL) {
					printf("there is not such an element");
					return;
				}
        node *tmp = NULL;
        if (T[index] == NULL || N == NULL) return;

        if (T[index]->key == key) {//если это голова
                T[index] = T[index]->next;
                free(N);
                N = NULL;
        } else {//ищем элемент перед N и перенаправляем указатель
                tmp = T[index];
                while (tmp->next && tmp->next != N) tmp = tmp->next;
                if (tmp->next) {
                        tmp->next = N->next;
                        free(N);
                        N = NULL;
                }
        }
}

void delete_table(node** T, int M){
       int i, j=0;
			 for( i = 0; i< M; i++)
			 		while(T[i][j].next != NULL)
						free(&T[i][j++]);
			 free(T);
}

void show_node(char* key, node** T, int M){
    node* n = search_by_key(key, T, M);
    printf("Key %s, data %p, hash(key)  %d\n", n->key, n->data, hash(key, M) );
}

