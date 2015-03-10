#include <stdio.h>
#include <stdlib.h>
#include "HT.h"

//#pragma warning (disable:4996)


int CompareStrings(char* string1, char* string2){
	int i = 0;
	while (1){
		if (string1[i] == string2[i]){
			if (string1[i] == '\0') return 1;
			else i++;
		}
		else return 0;
	}
};
int FileReadString(FILE* f, char* s)
{
	char t = 'a';
	while (t != '\0' && t != ' ' && t != '\n' && !feof(f)){
		fscanf(f, "%c", &t);
		if (t != '\0' && t != ' ' && t != '\n') (*s) = t;
		s++;
	}
	s[-1] = '\0';
	return 0;
}
int ReadString(char* s){
	char t = 'a';
	while (t != '\0' && t != ' ' && t != '\n'){
		scanf("%c", &t);
		if (t != '\0' && t != ' ' && t != '\n')
			(*s) = t;
		s++;
	}
	s[-1] = '\0';
	return 0;
}


struct node** CreateTable(int tsize){
	struct node** table = (struct node**)malloc(tsize*sizeof(struct node*)); int i;
	for (i = 0; i < tsize; ++i)
	{
		table[i] = (struct node*)malloc(sizeof(struct node));
		table[i]->next = NULL;
		table[i]->previous = NULL;
		table[i]->input = NULL;
	}
	return table;
}
void PrintTable(struct node** table, int tsize){
	int i;
	for (i = 0; i < tsize; ++i){
		printf("%d -->", i);
		struct node* p = table[i]->next;
		while (p != NULL){
			printf("%s<-->", p->input->KEY);
			p = p->next;
		}
		printf("NULL\n");
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
int Insert(struct node** table, int tsize, struct data* datain){
	int hash = HashU(datain->KEY, tsize);
	struct node* p = (struct node*)malloc(sizeof(struct node));

	p->next = table[hash]->next;
	p->previous = table[hash];

	if (p->next != NULL)
		p->next->previous = p;
	p->previous->next = p;


	p->input = datain;
	return 0;
}
int Delete(struct node** table, int tsize, key KEY){
	int hash = HashU(KEY, tsize);
	struct node* p = table[hash]->next;
	struct node* temp;
	while (p != NULL){
		temp = p->next;
		if (CompareStrings(p->input->KEY, KEY)){
			p->previous->next = p->next;
			if (p->next != NULL)
				p->next->previous = p->previous;
			free(p->input);
			free(p);
		}
		p = temp;
	}
	return 0;
}
void ClearTable(struct node** table, int tsize){
	int i;
	struct node *p, *temp;
	for (i = 0; i < tsize; ++i){
		p = table[i]->next;
		while (p != NULL){
			temp = p->next;
			if (p->input != NULL) free(p->input);
			free(p);
			p = temp;
		}
		table[i]->next = NULL;
	}
}
void DeleteTable(struct node** table, int tsize){
	int i;
	struct node *p, *temp;
	for (i = 0; i < tsize; ++i){
		p = table[i]->next;
		while (p != NULL){
			temp = p->next;
			if (p->input != NULL) free(p->input);
			free(p);
			p = temp;
		}
		free(table[i]);
	}
	free(table);
}