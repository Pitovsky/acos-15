int size;
#include"hash.h"

int main()
{
	int i,j,k,l,n; int *co;
	char *str;
	struct list** table; 
	FILE* test= fopen("test.txt", "r");
	FILE* out = fopen("output.txt", "w");
	fscanf(test,"%d", &size);
	table = MakeTable(size);
	str=(char*) malloc(sizeof(char)*250);
	for(i=0;i<size;i++)
	{
		fscanf(test, "%s", str);
		l=hash(str);	
		table[l]=Add(table[l], str, l);
	}
		
	fscanf(test,"%d", &n);
	for(i=0;i<n;i++)	
	{
		fscanf(test, "%s", str);
		co=Search(str,table);
		fprintf(out, "%d", *co);
	}
		
	fclose(test);
	fclose(out);		
	return(0);
}
