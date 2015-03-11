#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"hash-table.h"

typedef struct node{
    char* key;
    struct node* next;
} node;

node** create(int M){
    node** T = (node**)malloc(M*sizeof(node*));
    return T;
}

unsigned int hash(char* key, int M){
    return (int)key%M;
}
unsigned int hash1(char* key, unsigned int M)
   // unsigned int MurmurHash2
{
  const unsigned int m = 0x5bd1e995;
  const unsigned int seed = 0;
  const int r = 24;

  unsigned int h = seed ^ M;

  const unsigned char * data = (const unsigned char *)key;

  while (M >= 4)
    {
      unsigned int k;

      k  = data[0];
      k |= data[1] << 8;
      k |= data[2] << 16;
      k |= data[3] << 24;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      M -= 4;
    }

  switch (M)
    {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
    };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

void insert1(char *key, node** T, int M)
{
	int index = hash(key, M);
	T[index] = (node*)malloc(sizeof(node));
	if (T[index] != NULL) {
		T[index]->next = NULL;
		T[index]->key = key;
	}
}
/*void insert (char *key, node** T, int M)//добавляет элемент в голову T[index], старый сдвигается вглубь
{
	int index = hash(key, M);
	node *new_node = (node*)malloc(sizeof(node));
	if (new_node) {
        new_node->key = T[index]->key;
        new_node->next = T[index]->next;
		T[index]->key = key;
		T[index]->next = new_node;
	}
}
*/
node* search_by_key(char *key, node** T, int M){
    int index = hash(key, M);
    while (T[index]) {
		if (T[index]->key == key) break;
		T[index] = T[index]->next;
	}
	return T[index];
}

void delete(char* key, node **T, int M)
{
	int index = hash(key, M);
	node* N = search_by_key(key, T, M);
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
 	int i;
 	for (i = 0; i < M; i++){
        if (T[i]== NULL) continue;
        while (T[i] != NULL) {
            delete(T[i], T, M);
        }
	}
}

void show_node(char* key, node** T, int M){
    printf("%s %d\n", search_by_key(key, T, M)->key, hash(key, M) );
}
