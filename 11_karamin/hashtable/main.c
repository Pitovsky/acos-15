#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HT.h"

#pragma warning (disable:4996)

const int MAXCOUNT = 1000, tsize = 5, MaxStringSize = 100;

int main()
{

	FILE* in;
	in = fopen("input.txt", "r");
	if (in == NULL) return 1;

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
//C-Strings



	struct node** h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		Insert(h, tsize, input[i]);
	
	PrintTable(h, tsize);


	//Inserting
	//struct data *ins = (struct data*)malloc(sizeof(struct data));
	/*ins->data = ()malloc();
	printf("\n\nEnter inserting data --> ");
	scanf("%d", ins->data);*/
	//printf("\n\nEnter data key--> ");
	//key temp = (key)malloc(MaxStringSize*sizeof(char));
	//ReadString(temp);
	//ins->KEY = temp;
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