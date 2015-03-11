#include <stdlib.h>
#include <stdio.h>

struct list
{
    char* key;
    int value;
    struct list* next;
};

void  MakeTable(struct list*** table, int size_table)
{
    (*table) = (struct list**) malloc(sizeof(struct list*) * size_table);
    for (int i=0;i<size_table;i++)
    {
        (*table)[i] = NULL;
    }
    printf("Создана таблица размером %d\n", size_table);
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

void plus(struct list** table, char* a, int val, int size_table)
{
    struct list* help = (struct list*) malloc (sizeof(struct list));
    int ind = Hash(a,size_table);
    help->key = a;
    help->next = table[ind];
    table[ind] = help;
    help->value = val;
    printf("Добавлена строка %s - %d\n", a, val);
}

void Print (struct list** table, int size_table)
{
    printf("Печать таблицы...\n");
    struct list* d;
    for (int i=0;i<size_table;i++)
    {
        d = table[i];
        while (d != NULL)
        {
            printf("%s %d, ",d->key,d->value);
            d = d->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

int* Search_new(char* a, struct list** table, int size_table)
{
    printf("Поиск элемента %s...\n", a);
    int index = Hash(a, size_table);
    struct list* help = table[index];
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

void delete (struct list** table, char* a, int size_table)
{
    printf("Удаление элемента %s...\n", a);
    int ind = Hash(a, size_table);
    struct list* ex = (struct list*) malloc(sizeof(struct list)); 
    ex = table[ind];
    if (table[ind] != NULL)
    {
        if (table[ind]->key == a)
        {
            struct list* h = table[ind];
            table[ind] = table[ind]->next;
            free(h);
        }
        else
        {
            int key = 0;
            if (table[ind]->next != NULL)
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
                    struct list* help = ex->next;
                    ex->next = ex->next->next;
                    free(help);
                }
            }
        }
    }
}

void delete_table(struct list** table,int size_table)
{
    printf("Удаление таблицы...\n");
    struct list* help,*help_2;
    for (int i=0;i<size_table;i++)
    {
        if (table[i] == NULL)
        {
            free(table[i]);
        }
        else
        {
            help = table[i]->next;
	    free(table[i]->key);
            free(table[i]);
            while (help != NULL)
            {
                help_2 = help;
                help = help->next;
                free(help_2->key);
		free(help_2);
            }
        }
        table[i] = NULL;
    }
    free(table);
}
