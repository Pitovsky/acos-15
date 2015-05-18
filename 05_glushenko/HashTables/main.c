#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"

int main(){
	struct HashTable *table;
	FILE* f = fopen("input.txt", "r");
	if(f == NULL){
		perror("can't open file");
		_exit(EXIT_FAILURE);
	}
	int tabsize;
	fscanf(f, "%d", &tabsize);
	
	table = CreateHashtable(tabsize);
	char* input = malloc(256*sizeof(char));
	
	int code = 0;
	while(code != EOF){
		char CurCode[10];
		int value;
		code = fscanf(f, "%s", CurCode);
		if(code == EOF)
			break;
		if(code != 1){
			DeleteHashtable(&table);
			free(input);
			fclose(f);
			return 1;
		}
		switch(*CurCode){
			case 'a':
				code = fscanf(f, "%d %s", &value, input);
				
				char* data = malloc(strlen(input));
				data = strcpy(data, input);
				int* val = malloc(sizeof(int));
				*val = value;
				InsertData(table, data, val);
				printf("added %s, %d\n", input, value);
				break;
			case 'f':
				code = fscanf(f, "%s", input);
				int *found = GetData(table, input);
				if(found == 0)
					printf("Can't find");
				else
					printf("key: %s Val: %d\n", input, *found);
					break;
			case 'r':
				code = fscanf(f, "%s", input);
				DeleteData(table, input);
				printf("deleted: %s\n", input);
				break;
		}
	}
	DeleteHashtable(&table);
	free(input);	
	fclose(f);
	return 0;
} 
