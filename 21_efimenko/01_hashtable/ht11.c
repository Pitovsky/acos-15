#include"ht11.h"



int hash_func(int size, char* key)
{
	int i = 0;
	int sum = 0;
	for (i = 0; i < strlen(key); ++i){
		sum = sum + (int)key[i];
	}
	sum = sum % size;
	return sum;
}

Table* createTable(size_t size){
	Table* t = (Table*)malloc(sizeof(Table));
	t->size = size;
	t->buckets = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; ++i){
		t->buckets[i] = NULL;
    }
    return t;
}


void clearTable(Table *t)
{
    for (int i = 0; i < t->size; ++i)
        if (t->buckets[i] != NULL)
        {
printf("ok\n");
            Node *h = t->buckets[i];
printf("ok\n");
            while (h != NULL)
            {
		h = h->next;
		if(h->key != NULL)
                free(h->key);
		
            }
            free(t->buckets[i]);
        }
    free(t->buckets);
}

Node* findNode (char* key, Table* t) {
    Node *p;
    p = t->buckets[hash_func(t->size,key)];
    while (1) {
	if (p == 0x0)
		return p;
    if (strcmp(p->key, key) == 0) {
		printf("I found %s with key = %d\n", key, hash_func(t->size, key));
		return p;
	}
	else
        p = p->next;
	}
    return p;
}

Node* insertNode(char* key, Table* t) {
    Node *p1, *p2;
    int k;
    k = hash_func(t->size, key);
    if ( (p1 = (Node*)malloc(sizeof(Node))) == NULL)
	{
        fprintf (stderr, "out of memory (insert)\n");
        exit(1);
    }
    p2 = t->buckets[k];
    p1->next = p2;
//	p1->key = (char*)malloc(strlen(key)*sizeof(char));
    p1->key = key;
    t->buckets[k] = p1;
    return p1;			// our Node with data
}

void deleteNode(char* key, Table* t) {
    Node *p1, *p2;
    int k;
    p2 = 0;
    k = hash_func(t->size, key);
    p1 = t->buckets[k];
    while (p1 && !(p1->key == key))		// finding data in our list
	{
        p2 = p1;
        p1 = p1->next;
    }
    if (!p1) 
		return;
    if (p2)
        p2->next = p1->next;
    else
        t->buckets[k] = p1->next;
    free(p1);
	free(p2);
}


