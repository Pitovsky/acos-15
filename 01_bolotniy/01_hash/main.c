#include<stdio.h>

int main(){
	struct list* a = CreateTable();
	int n = 2;
	int i;
	int item;
	char key[30];
	for (i = 0; i < n; i++){
		scanf("%d%s",&item,key);
		ElementAdd(a,item,key);
		printf("%d\n",hash(key));
	}
	ShuntPrintTable(a);
	scanf("%s",key);
	ElementSearch(a,key);
	ShuntPrintTable(a);
	ElementDel(a,key);
	ShuntPrintTable(a);
	DeleteTable(a);
	return(0);
}
