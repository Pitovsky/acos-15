#include"hash.h"

int main()
{
	int i,j,k,l,n; int *co;
	char *str;
	struct list** table; 
	FILE* test= fopen("test.txt", "r");
	FILE* out = fopen("output.txt", "w");
	fscanf(test,"%d", &n);
	table = MakeTable(n);
	str=(char*) malloc(sizeof(char)*250);
	for(i=0;i<n;i++)
	{
		fscanf(test, "%s", str);
		l=hash(str,n);	
		table[l]=Add(table[l], str, l);
	}
		//deleteTable(table);

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
