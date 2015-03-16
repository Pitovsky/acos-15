#include<stdio.h>

int main(){
	FILE *f = fopen("input.txt", "r");
	if (f == NULL) {return 1;}

	struct list* a = CreateTable();
	int item;
	int buf;
	char key[30];
	int comand;
	comand = fgetc(f);
	while (comand != EOF){
	
		if(comand == 'a'){
			fscanf(f, "%s%d", key, &item);
			ElementAdd(a, item, key);
		}

		if(comand == 's'){
			fscanf(f, "%s", key);
			ElementSearch(a, key);
		}

		if(comand == 'd'){
			fscanf(f, "%s", key);
			ElementDel(a, key);
		}

		if(comand == 'p'){
			ShuntPrintTable(a);
		}

		if(comand == 'h'){
			fscanf(f, "%s", key);
			buf = hash(key);
			printf("%d", buf);		
		}

		comand = fgetc(f);
	}
	fclose(f);
	DeleteTable(a);
	return(0);
}
