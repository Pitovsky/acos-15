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
        result[i] = ' ';
    }
    return result;
};


int hash(char* v, int n){	
	int i=0,j;
	int code=0;
	
	for(j=0;j<strlen(v);j++){
        	i = (i + v[j]) % n;
    	}
	return(i);	
};


struct list* Add(struct list* to,char* v, int n){
    struct list* now = (struct list*) malloc(sizeof(struct list));
    now->key = v;
    now->next = to;
    now->value = n;
	printf("%s added as %d", v,n);
    return now;
};

int main()
{
	int i,j,k,l,n;
	char *str;
	struct list** table = MakeTable(300);
	//scanf("%d", &n);
	//for(i=0;i<n;i++)
		scanf("%s", str);
		l=hash(str, 300);	
		table[l]=Add(table[l], str, 300);
	return(0);
}
