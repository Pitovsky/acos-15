#include <stdio.h>
#include <stdlib.h>
#include <string>

#define maxsize 1000000;
typedef int hashTableIndex;
typedef char* T;

typedef struct Node_ {
	struct Node_ *next = NULL;         
	T string;                     
} Node;

Node** hashTable;

hashTableIndex hash(T s) 
{
	const int p = 31;
	long long hash = 0, p_pow = 1;
	for (size_t i = 0; s[i]!=0; ++i)
	{
		hash += (s[i] - 'a' + 1) * p_pow;
		p_pow *= p;
	}
	return hash % maxsize;
};

Node** CreateHashTable()
{
	int n = maxsize;
	Node** a = (Node**)malloc(sizeof(Node*)*n);
	for (int i = 0; i < n; i++) a[i] = NULL;
	return a;
}

void InsertNode(Node **hashTable, T s)
{
	int Index = hash(s);
	Node*p = hashTable[Index];
	while (p != NULL) p = (*hashTable[Index]).next;
	(*p).string = s;
	(*p).next = (Node*)malloc(sizeof(Node*));
}

Node* FindNode(Node **hashTable, T s)
{
	int Index = hash(s);
	Node*p0;
	p0 = hashTable[Index];
	while ((strcmp((*p0).string, s) != 0) || (p0 != NULL)) p0 = (*p0).next;
	if (p0 == NULL)
	{
		printf("Not found");
		return NULL;
	}
	else return p0;
}

void DeleteNode(Node **hashTable, T s)
{
	int Index = hash(s);
	int k = 0;
	Node*p0;
	p0 = hashTable[Index];
	while ((strcmp((*p0).string, s) != 0) || (p0 != NULL))
	{
		p0 = (*p0).next;
		k++;
	}
	if (p0 == NULL) printf("Not found");
	else
	{
		if (k == 0) p0 = NULL;
		else
		{
			while ((*p0).next != NULL)
			{
				(*p0).string = (*((*p0).next)).string;
				p0 = (*p0).next;
			}
			free(p0);
		}		
	};
}

void DeleteHashTable(Node **hashTable)
{
	Node *p0, *p, *p1;
	int k=0;
	int n = maxsize;
	for (int i = 0; i < n; i++)
	{
		p0 = hashTable[i];
		if (p0 != NULL)
		{
			p = (*p0).next;
			while ((*p).next != NULL)
			{
				p1 = (*p).next;
				if (k != 0) free(p);
				p = p1;
				k++;
			}
			if (k != 0) free(p);
		}
	}
	free(*hashTable);
}