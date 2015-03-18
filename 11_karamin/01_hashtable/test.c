#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HT.h"

//#pragma warning (disable:4996)

const int MaxStringSize = 100;

int main()
{
	char* name = (char*)malloc(10 * sizeof(char));
	printf("Enter testfile name (test1-4) --> ");
	scanf("%s", name);

	FILE* in;
	in = fopen(name, "r");
	if (in == NULL) 
		return 1;

	int tsize;
	printf("\nEnter table size --> ");
	scanf("%d", &tsize);

//Entering Data with a key from file
	int count, i, NumberOfInputData;
	fscanf(in, "%d", &NumberOfInputData);
	struct data** input = (struct data**)malloc(NumberOfInputData*sizeof(struct data*));
	for (i = 0; i < NumberOfInputData; ++i){
		input[i] = (struct data*)malloc(MaxStringSize*sizeof(struct data));
		input[i]->KEY = (char*)malloc(MaxStringSize*sizeof(char));
		fscanf(in, "%s %d", input[i]->KEY, &(input[i]->data));
	}
	count = i;
//C-Strings


//Inserting data in the table
	struct table* h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		InsertInTheTable(h, input[i]);	
	printf("\nTable created and filled from %s.", name);
	//PrintTable(h, tsize);
//Inserting data in the table


//Inserting in the table from keyboard
	struct data *ins = (struct data*)malloc(sizeof(struct data));
	printf("\n\nEnter data to insert: ");
	key KEY = (key)malloc(MaxStringSize*sizeof(char));
	int data;
	printf("Enter key --> ");
	scanf("%s", KEY);
	printf("\nEnter data(int) --> ");
	scanf("%d", &data);

	ins->KEY = KEY;
	ins->data = data;
	InsertInTheTable(h, ins);

	printf("\n\n");
	//PrintTable(h, tsize);
//Inserting in the table from keyboard


//Finding in the table
	struct data* p;
	key Key = (key)malloc(MaxStringSize*sizeof(char));
	printf("\nEnter key to find data --> ");
	scanf("%s", Key);
	p = FindInTheTable(h, Key);
	printf("Finded data --> ");
	if (p == NULL) printf("NULL");
	else printf("%d", p->data);
//Finding in the table


//Printing data by hash
	int hash;
	printf("\n\nEnter hash to print data -->");
	scanf("%d", &hash);
	PrintList(h->mytable[hash]);
//Printing data by hash


//Deleting element from table
	key del = (key)malloc(100 * sizeof(char));
	printf("\n\nEnter deleting string --> ");
	scanf("%s", del);
	DeleteFromTable(h, del);

	//printf("\n\nModified table:\n");
	//PrintTable(h, tsize);
//Deleting element from table


//Clearing and deleting table
	ClearTable(h);
	printf("\nTable cleared!\n");
	//PrintTable(h);
	DeleteTable(h);
//Clearing and deleting table

	printf("\nAll good.\n");
	
//	system("pause");
	return 0;
}
