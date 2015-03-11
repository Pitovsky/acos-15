#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct list{
    char* key;
    int value;
    struct list* next;
};


struct list** MakeTable(int n){
    int i;
    struct list** result = (struct list**) malloc(sizeof(struct list*)*n);
    for(i=0;i<n;i++){
        result[i] = NULL;
    }
    return result;
};


int hash(char* v, int n){	
	int i=0,j;
	int code=0;
	
	for(j=0;j<strlen(v);j++){
        	i = (i+v[j]) % n;
    	}
	return(i);	
};


struct list* Add(struct list* to,char* v, int n){
    struct list* now = (struct list*) malloc(sizeof(struct list));
    now->key = v;
    now->next = to;
    now->value = n;
	printf("%s added as %d\n", v,n);
    return now;
};

void delete(struct list** table, char* v){
	int ind=hash(v,300);
        struct list* ex = table[ind];
        if (table[ind] != NULL){
        	if (table[ind]->key == v){
            		struct list* h = table[ind];
            		table[ind] = table[ind]->next;
            		free(h);
        	}
        	else{
            	int key = 0;
            	if (table[ind]->next != NULL){
                	while ((ex->next->next != NULL) && (key == 0)){
                    		if (ex->next->key == v){key = 1;}
                    		else{ex = ex->next;}         
			}
                	if (key != 0){
                    		struct list* help = ex->next;
                    		ex->next = ex->next->next;
                    		free(help);
                	}
            	}
                }
        }
	printf("Mymory free now\n");
}	

void deleteTable(struct list** table){
	int i,j;
	struct list *one, *two;
	for(i=0;i<300;i++)
		free(table[i]);
	printf("Table free now\n");
}

int main()
{
	int i,j,k,l,n;
	char *str;
	struct list** table = MakeTable(300);
	str=(char*) malloc(sizeof(char)*250);
	//scanf("%d", &n);
	//for(i=0;i<n;i++)
		scanf("%s", str);
		l=hash(str, 300);	
		table[l]=Add(table[l], str, l);
		deleteTable(table);		
	return(0);
}
