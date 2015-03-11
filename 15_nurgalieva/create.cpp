#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*нужны операции добавления в начало списка - есть, удаление из любого места , удаление самого списка - есть, создание списка - -есть*/

struct Node
{
	struct Node * next;
	struct Node* prev;
	const char* value;
};

struct List {
	struct Node * head;
	struct Node * tail;
};

/*создание пустого списка*/
struct List* createList()
{
	struct List* list = (struct List*)malloc(10 * sizeof(struct List));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

/*удаление списка*/
void deleteList(struct List** list) /*моя функция удаления листа*/
{
	struct Node* tmp = NULL;
	while ((*list)->head)
	{
		tmp = (*list)->head;
		(*list)->head = (*list)->head->next;
		free(tmp);
	}
	free(*list);
	return;
}

/*поиск в списке, возвращает указатель на узел, если нашёл, и 0, если не нашел*/
struct Node* findInto(struct List* list, const char* str)
{
	struct Node * tmp = list->head;
	while (tmp)
	{
		if (str == tmp->value)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

/*удаление из любого места списка*/
void DeleteFromList(struct List list, const char * str, size_t size)
{
	//struct Node* tmp = findInto(list, str , list->size);
	//if (tmp->next);
}

/*вставка в начало списка*/
void InsertIntoHead(struct List* list, const char* value)
{
	struct Node* tmp = (struct Node*)malloc(sizeof(struct Node*));
	tmp->value = value;
	tmp->next = list->head;
	tmp->prev = NULL;
	if (list->head)
	{
		list->head->prev = tmp;
	}
	list->head = tmp;
	if (!list->tail)
	{
		list->tail = tmp;
	}
	return;
}

/*функция для печати значений списка*/
void print(const char* string, size_t index)
{
	printf("(%d) -> %s \n", index, string);
	return;
}

/*печать списка*/
void printList(struct List *list, void(*print)(const char*, size_t))
{
	struct Node * tmp = list->head;
	size_t index = 1;
	while (tmp != NULL)
	{
		print(tmp->value, index);
		index++;
		tmp = tmp->next;
	}
	return;
}

void TestFanc()
{
	struct List* list = createList();
	InsertIntoHead(list, "Andrey");
	//list = realloc(list, 2*sizeof(struct List));
	InsertIntoHead(list, "Olya");
	printList(list, print);
	struct Node* tmp = findInto(list, "Andrey");
	printf("%s \n", tmp->prev->value);
	list_delete(&list);
	//deleteList(list);
	//printList(&list, print);
	return;
}

/*void readFile(FILE* f)
{
string* str = (string*)malloc(sizeof(string));
int i = 0;
while (!EOF)
{
fscanf(f, )
}
}*/

int main()
{
	TestFanc();
	//FILE* f = fopen("input", r);
	//readFile(f);
	return 0;
}

