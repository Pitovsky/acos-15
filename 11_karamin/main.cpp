#include <cstdio>
#include <cstdlib>
//#include <string.h>

//#pragma warning (disable:4996)

const int MAXCOUNT = 1000, tsize = 3;

typedef char* key;

struct node{
	node* p;
	node* next;
	node* previous;
	char* string;
};

int CompareStrings(char* string1, char* string2){
	int i = 0;
	while (1){
		if (string1[i] == string2[i]){
			if (string1[i] == '\0') return 1;
			else i++;
		}
		else return 0;
	}
}
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

node** CreateTable(int tsize){
	node** table = (node**)malloc(tsize*sizeof(node*)); int i;
	for (i = 0; i < tsize; ++i)
	{
		table[i] = (node*)malloc(sizeof(node));
		table[i]->next = NULL;
		table[i]->previous = NULL;
	}
	return table;
}
void PrintTable(node** table, int tsize){
	int i;
	for (i = 0; i < tsize; ++i){
		printf("%d -->", i);
		node* p = table[i]->next;
		while (p != NULL){
			printf("%s<-->", p->string);
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
int Insert(node** table, int tsize, key KEY){
	int hash = HashU(KEY, tsize);
	node* p = (node*)malloc(sizeof(node));
	
	p->next = table[hash]->next;
	p->previous = table[hash];
	
	if (p->next != NULL) 
		p->next->previous = p;
	p->previous->next = p;
	
	p->string = KEY;
	return 0;
}
int Delete(node** table, int tsize, key KEY){
	int hash = HashU(KEY, tsize), i;
	node* p = table[hash]->next;
	node* temp;
	while (p != NULL){
		temp = p->next;
		if (CompareStrings(p->string, KEY)){
			p->previous->next = p->next;
			if (p->next != NULL)
				p->next->previous = p->previous;
			free(p);
		}
		p = temp;
	}
	return 0;
}
void ClearTable(node** table, int tsize){
	int i;
	node *p, *temp;
	for (i = 0; i < tsize; ++i){
		p = table[i]->next;
		while (p != NULL){
			temp = p->next;
			free(p);
			p = temp;
		}
		table[i]->next = NULL;
	}
}
void DeleteTable(node** table, int tsize){
	int i;
	node *p, *temp;
	for (i = 0; i < tsize; ++i){
		p = table[i];
		while (p != NULL){
			temp = p->next;
			free(p);
			p = temp;
		}
	}
	free(table);
}


int main()
{

	FILE* in;
	in = fopen("input.txt", "r");

//C-Strings
	int count, lth = 100, i;
	node** table = (node**)malloc(tsize*sizeof(node*));
	key* strings = (char**)malloc(MAXCOUNT*sizeof(char*));
	for (i = 0; !feof(in); ++i){
		strings[i] = (char*)malloc(lth*sizeof(char));
		FileReadString(in, strings[i]);
	}
	count = i;
//C-strings

	node** h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		Insert(h, tsize, strings[i]);
	
	PrintTable(h, tsize);

	//key ins = (key)malloc(100 * sizeof(char));
	//printf("\n\nEnter inserting string --> ");
	//scanf("%s", ins);
	//Insert(h, tsize, ins);

	//printf("\n\n");
	//PrintTable(h, tsize);

	//key del = (key)malloc(100 * sizeof(char));
	//printf("\n\nEnter deleting string --> ");
	//scanf("%s", del);
	//Delete(h, tsize, del);

	//printf("\n\n");
	//PrintTable(h, tsize);
	
	//ClearTable(h, tsize);
	//DeleteTable(h, tsize);

	system("pause");
	return 0;
}