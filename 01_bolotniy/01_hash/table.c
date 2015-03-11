#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node{
	int item;
	char *key;
	struct node *next;	
};

struct list{
	struct node *first;
};

void ListAdd(struct list *a,struct node *b){
	b->next = a->first;
	a->first = b;
}

int hash (char *a){
	int i;
	int res = 0;
	for(i=0; a[i] != '\0'; i++){
		res = (res+a[i]) % 256;
	}
	return(res);
}

struct list * CreateTable (){
	struct list *a = (struct list *)malloc(256 * sizeof(struct list));
	int i;
	for(i=0; i<256; i++){
		a[i].first = NULL;
	}
	return(a);
}

void DeleteTable (struct list *table){
	int i;
	struct node *a;
	for(i=0; i<256; i++){
		while (table[i].first != NULL){
			a = table[i].first;
			table[i].first=table[i].first->next;
			free(a);
		}
	}
	free(table);
}

void ElementAdd (struct list *table, int item, char *key){
	int ind = hash(key);
	struct node *a =(struct node *)malloc(sizeof(struct node));
	a->item = item;
	a->key = key;
	a->next = NULL;
	ListAdd(&(table[ind]), a);
}

int ElementSearch (struct list *table, char *key){
	int ind = hash(key);
	int f = -1;
	int item;
	struct list a = table[ind];
	while(a.first != NULL){
		if (strcmp(a.first->key,key) == 0 ){ 
			f = 0;
			item = a.first->item;
			a.first=a.first->next;
		}else{
			a.first=a.first->next;
		}
	}
	if (f == -1){printf("NOT_FOUND\n");}else{printf("%d\n",item);}
}

void ElementDel (struct list *table, char *key){
	int ind = hash(key);
	int f = 0;
	struct node *a = table[ind].first;
	struct node *buf;
	if (a != NULL){
		if (strcmp(a->key,key) == 0){
			f = -1;
			table[ind].first = a->next;
			free(a);
		}else{
			while((a->next != NULL) && (f == 0)){
				if (strcmp((a->next)->key,key) == 0){
					f = -1;
					buf = a->next;
					a->next = (a->next)->next;
					free(buf);
				}else{
					a = a->next;
				}
			}
		}
	}
	if(f == 0){printf("WAS_NOT_DELEATED\n");}
}

void ShuntPrintTable(struct list *table){
	int i;
	struct list a;
	for(i = 0; i < 256; i++){
		if (table[i].first != 0){
			printf("%d ",i);
			a = table[i];
			while (a.first != NULL){
				printf("%d ",a.first->item);
				a.first = a.first->next;
			}
		printf("\n");
		}
	}
}