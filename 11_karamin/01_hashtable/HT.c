#include <stdio.h>
#include <stdlib.h>
#include "HT.h"

#pragma warning (disable:4996)

struct table* CreateTable(int tsize)
{
	int i;
	struct table* Table = (struct table*)malloc(sizeof(struct table));
	Table->Size = tsize;
	Table->NumberOfElements = 0;
	Table->mytable = (struct node**)malloc(tsize*sizeof(struct node*));
	for (i = 0; i < tsize; ++i) Table->mytable[i] = NULL;
	return Table;
}
int InsertInTheTable(struct table* Table, struct data* data)
{
	int hash = HashU(data->KEY, Table->Size);
	struct node* temp = Table->mytable[hash];

	struct node* p = (struct node*)malloc(sizeof(struct node));
	p->input = data;
	p->next = temp;
	p->previous = NULL;
	if (temp != NULL) temp->previous = p;
	Table->mytable[hash] = p;

	Table->NumberOfElements++;
}
struct data* FindInTheTable(struct table* table, key KEY)
{
	int hash = HashU(KEY, table->Size);
	struct node* p = table->mytable[hash];
	while (p != NULL && strcmp(p->input->KEY, KEY) != 0)
		p = p->next;
	if (p != NULL) return p->input;
	else return NULL;
}
int DeleteFromTable(struct table* table, key KEY)
{
	int hash = HashU(KEY, table->Size);
	struct node* p = table->mytable[hash];
	struct node* temp; char deleted = 0;
	while (p != NULL)
	{
		temp = p->next;
		if (!strcmp(p->input->KEY, KEY))
		{
			if (p->previous != NULL)
				p->previous->next = p->next;
			else
				table->mytable[hash] = temp;

			if (p->next != NULL)
				p->next->previous = p->previous;

			free(p->input);
			free(p);
			table->NumberOfElements--;

			printf("\n%s was deleted.", KEY);
			deleted = 1;
		}
		p = temp;
	}
	if (!deleted) printf("Nothing was deleted...");
	return 0;
}

void PrintTable(struct table* table){
	int i, tsize = table->Size;
	struct node* p;
	
	for (i = 0; i < tsize; ++i){
		p = table->mytable[i];
		printf("(Hash == %d) -->", i);
		while (p != NULL)
		{
			printf("%s<-->", p->input->KEY);
			p = p->next;
		}
		printf("NULL\n");
	}
}
void PrintList(struct node* list)
{
	while (list != NULL)
	{
		printf("%d\t", list->input->data);
		list = list->next;
	}
}
int HashU(char* s, int M)
{
	int a = 31415, b = 27183, h = 0;
	for (; (*s) != '\0'; (s++), a = a*b % (M - 1))
	{
		h = (a*h + *s) % M;
	}
	return h;
}

void ClearTable(struct table* table){
	int i;
	struct node *p, *temp;
	for (i = 0; i < table->Size; ++i){
		p = table->mytable[i];
		while (p != NULL){
			temp = p->next;
			if (p->input != NULL)
				free(p->input);
			free(p);
			p = temp;
		}
		table->mytable[i] = NULL;
	}
	table->NumberOfElements = 0;
}
void DeleteTable(struct table* table){
	ClearTable(table);
	free(table->mytable);
	free(table);
	table = NULL;
}