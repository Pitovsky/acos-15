#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HT.h"

//#pragma warning (disable:4996)

const int MAXCOUNT = 1000, tsize = 5, MaxStringSize = 100;

int main()
{
	FILE* in;
	in = fopen("keys.txt", "r");
	if (in == NULL) return 1;

//________Filling table from input.txt________
//Entering Data with a key
	int count, i/*, DataTypeSize*/;
	//fscanf(in, "%d", &DataTypeSize);
	struct data** input = (struct data**)malloc(MAXCOUNT*sizeof(struct data*));
	for (i = 0; !feof(in); ++i){
		input[i] = (struct data*)malloc(MaxStringSize*sizeof(struct data));
		input[i]->KEY = (char*)malloc(MaxStringSize*sizeof(char));
		/*fscanf(in, "%d", &input[i]->data);*/ FileReadString(in, input[i]->KEY);
	}
	count = i;

	struct node** h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		Insert(h, tsize, input[i]);
	
	PrintTable(h, tsize);
//________Filling table from input.txt________

//____________Inserting_______________
	struct data *ins = (struct data*)malloc(sizeof(struct data));
	/*ins->data = ()malloc();
	printf("\n\nEnter inserting data --> ");
	scanf("%d", ins->data);*/
	printf("\n\nEnter data key--> ");
	key temp = (key)malloc(MaxStringSize*sizeof(char));
	ReadString(temp);
	ins->KEY = temp;
	Insert(h, tsize, ins);

	printf("\n\n");
	PrintTable(h, tsize);
//____________Inserting_______________

//____________Deleting________________
	key del = (key)malloc(100 * sizeof(char));
	printf("\n\nEnter deleting string --> ");
	scanf("%s", del);
	Delete(h, tsize, del);

	printf("\n\n");
	PrintTable(h, tsize);
//____________Deleting________________

	ClearTable(h, tsize);
	DeleteTable(h, tsize);

	return 0;
}
