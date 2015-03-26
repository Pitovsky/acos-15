#include <stdlib.h>
#include <stdio.h>

typedef struct list list;
struct list
{
    char* key;
    int value;
    list* next;
};

typedef struct s_table s_table;
struct s_table
{
    list** list;
    int size;
};

void  MakeTable(s_table* table, int* size_table)
{
    printf("%d\n", *size_table);
    int size = *size_table;
    table->list = (list**) malloc(sizeof(list*) * size);
    int i;
    printf("%d\n",size);
    for (i=0;i<size;i++)
    {
	table->list[i] = NULL;
    }
    table->size = size;
    printf("Создана таблица размером %d\n", size);
}

int Hash (char* v,int size_table)
{
    int h = 0;
    int a = 127;
    for (; *v!=0; v++)
    {
        h = (a*h + *v) % size_table;
    }
    return h;
}

void plus(s_table table, char* a, int val)
{
    list* help = (list*) malloc (sizeof(list));
    int ind = Hash(a, table.size);
    help->key = a;
    help->next = table.list[ind];
    table.list[ind] = help;
    help->value = val;
    printf("Добавлена строка %s - %d\n", a, val);
}

void Print (s_table table)
{
    printf("Печать таблицы...\n");
    list* d;
    for (int i=0;i<table.size;i++)
    {
        d = table.list[i];
        while (d != NULL)
        {
            printf("%s %d, ",d->key,d->value);
            d = d->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

int* Search_new(char* a, s_table table)
{
    printf("Поиск элемента %s...\n", a);
    int index = Hash(a, table.size);
    list* help = table.list[index];
    int _key = 0;
    while ((help != NULL) && (_key == 0))
    {
        if (help->key == a)
        {
            _key = 1;
        }
        else
        {
            help = help->next;
        }
    }
    
    if (_key == 1)
    {
        return (help->value);
    }
    else
    {
        return NULL;
    }
}

void delete (s_table table, char* a)
{
    printf("Удаление элемента %s...\n", a);
    int ind = Hash(a, table.size);
    list* ex = (list*) malloc(sizeof(list)); 
    ex = table.list[ind];
    if (table.list[ind] != NULL)
    {
        if (table.list[ind]->key == a)
        {
            list* h = table.list[ind];
            table.list[ind] = table.list[ind]->next;
            free(h);
        }
        else
        {
            int key = 0;
            if (table.list[ind]->next != NULL)
            {
                while ((ex->next->next != NULL) && (key == 0))
                {
                    if (ex->next->key == a)
                    {
                        key = 1;
                    }
                    else
                    {
                        ex = ex->next;
                    }
                }
                if (key != 0)
                {
                    list* help = ex->next;
                    ex->next = ex->next->next;
                    free(help);
                }
            }
        }
    }
}

void delete_table(s_table table)
{
    printf("Удаление таблицы...\n");
    list* help,*help_2;
    for (int i=0;i<table.size;i++)
    {
        if (table.list[i] == NULL)
        {
            free(table.list[i]);
        }
        else
        {
            help = table.list[i]->next;
	    free(table.list[i]->key);
            free(table.list[i]);
            while (help != NULL)
            {
                help_2 = help;
                help = help->next;
                free(help_2->key);
		free(help_2);
            }
        }
        table.list[i] = NULL;
    }
    free(table.list);
}
