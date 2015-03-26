#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
    char* key;
    int value;
    struct list* next;
}list;

typedef struct s_table
{
    list** list;
    int size;
}s_table;

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    int size_table;
    fscanf(fp,"%d",&size_table);
    fscanf(fp, "%d",&size_table);
    printf("%d\n",size_table);
    s_table table1;
    printf("hello maketable\n");
    MakeTable(&table1, &size_table);
    printf("%d\n",table1.size); 
    int stream;
    int val;
    int* search;
    int code = 0; 
    char* key;
    while (code != EOF)
    {
	code = fscanf(fp, "%d", &stream);
        if (code == EOF)
	    break;
        if (stream == 1)
	{
	    key = (char*) malloc (sizeof(char)*20);	
	    fscanf(fp, "%s %d", key, &val);
	    plus(table1, key, val);	    
	}
	if (stream == 2)
	{
	    Print(table1);
	}
	if (stream == 3)
	{
	    fscanf(fp, "%s", key);
	    search = Search_new(key, table1);
	    if (search == NULL)
	    {
		printf("Элемент %s не найден\n",key);
	    }
	    else
	    {
		printf("Элемент найден: %d\n",(int) search);
	    }
	}
        if (stream == 4)
        {
	    fscanf(fp, "%s", key);
	    delete(table1, key);
	}
	if (stream == 5)
	{
    	    delete_table(table1);
	} 
    }
    
    return 0;
}

