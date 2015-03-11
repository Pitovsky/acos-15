#include <stdio.h>
#include <stdlib.h>


int main(int argv, char* argc[])
{
    FILE* fp = fopen(argc[1], "r");
    int size_table;
    fscanf(fp,"%d",&size_table);
    fscanf(fp, "%d",&size_table);
    struct list** table;
    MakeTable(&table, size_table);
    
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
	    plus( table, key, val, size_table);	    
	}
	if (stream == 2)
	{
	    Print(table, size_table);
	}
	if (stream == 3)
	{
	    fscanf(fp, "%s", key);
	    search = Search_new(key, table, size_table);
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
	    delete(table, key, size_table);
	}
	if (stream == 5)
	{
    	    delete_table(table, size_table);
	} 
    }
    //plus(table, "d", 34, size_table);

    //plus(table, "fghf", 45, size_table);

    //plus(table, "dfhtjrs", 666, size_table);
    
    //printf("\n");
    //Print(table, size_table);

    //int* a;
    //a = Search_new("fghf", table, size_table);
    
    //if (a == NULL)
    //{
    //    printf("Неудача\n");
    //}
    //else
    //{
    //    printf("%d\n",(int) a);
    //}
    
    //printf("\n");
    //delete(table, "dfhtjrs",size_table);
    
    //Print(table,size_table);
    //delete_table(table,size_table);

    //Print(table,size_table);
    return 0;
}

