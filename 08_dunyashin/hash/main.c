#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

int PrimalFunction(char* key, int border){
	size_t i = 0;
	int result = 0;
	while(key[i] != '\0' && key[i] != '\n'){
		result += (int)key[i];
		result %= border;
		++i;
	}
	return result;
}

char* ScanString(){
	int c = getchar(), length = 0, maxlength = 10;
	char* string = (char*)malloc(maxlength * sizeof(char));
	while (c != '\n'){
		if (length == maxlength - 2){
			maxlength *= 2;
			string = (char*)realloc(string, maxlength);
		}
		string[length] = (char)c;
		length += 1;
		c = getchar();
	}
	string[length] = '\0';
	return string;
}

int AskPerson(){
	printf("Choose an action:\n");
	printf("1) Add something\n");
	printf("2) Get something\n");
	printf("3) Delete something\n");	
	printf("4) End\n");
	char* input = ScanString();
	if (strlen(input) == 1){
		if (input[0] == '1'){
			free(input);
			return 1;
		}
		if (input[0] == '2'){
			free(input);
			return 2;
		}
		if (input[0] == '3'){
			free(input);
			return 3;
		}
		if (input[0] == '4'){
			free(input);
			return 4;
		}
	}
	free(input);
	return 0;
}

void DoAdd(struct Table* table, int (*hashfunc)(char* key, int border)){
	printf("Enter a key:");
	char* key = ScanString();
	printf("Enter a value:");
	int value;
	scanf("%d", &value);
	printf("Value has been scaned\n");
	AddToTable(table, key, value, hashfunc);
	printf("Added\n");
	return;
}

int DoGet(struct Table* table, int* value, int (*hashfunc)(char* key, int border)){
	printf("Enter a key:");
	char* key = ScanString();
	int result = GetFromTable(table, key, value, hashfunc);
	free(key);
	return result;
}

void DoDelete(struct Table* table, int (*hashfunc)(char* key, int border)){
	printf("Enter a key:");
	char* key = ScanString();
	DeleteFromTable(table, key, hashfunc);
	free(key);
	return;
}

int main(){
	int a = 0, result_value = 0, result_function;
	struct Table* HashTable = New_Table(100);
	while ( a != 4){
		a = AskPerson();
		switch(a){
		case 1:
			DoAdd(HashTable, &PrimalFunction);
			break;
		case 2:
			result_function = DoGet(HashTable, &result_value, &PrimalFunction);
			if (result_function == 0){
				printf("Value was gotten: %d\n", result_value);
			}
			else{
				printf("Value wasn't gotten\n");
			}
			break;
		case 3:
			DoDelete(HashTable, &PrimalFunction);
			break;
		default:
			break;
		}
	}
	return 0;
}
