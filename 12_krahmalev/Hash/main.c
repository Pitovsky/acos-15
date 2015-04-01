#include"hash.h"

int main()
{
	int i,j,k,l,n; int *co;
	char *str;
	struct list** table; 
	table = MakeTable(277);
	str=(char*) malloc(sizeof(char)*250);
	//scanf("%d", &n);
	//for(i=0;i<n;i++)
		scanf("%s", str);
		l=hash(str);	
		table[l]=Add(table[l], str, l);
		//deleteTable(table);
		co=Search(str,table);
		printf("%d", *co);		
	return(0);
}
