# include "stdafx.h"
//#ifndef "create.h"
//#define "create.h"
//#endif;

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


struct List* createList();

/*удаление списка*/
void deleteList(struct List** list) /*моя функция удаления листа*/;

/*удаление из любого места списка*/
void DeleteFromList(struct List* list, const char * str);

/*поиск в списке, возвращает указатель на узел, если нашёл, и 0, если не нашел*/
struct Node* findInto(struct List* list, const char* str);

/*вставка в начало списка*/
void InsertIntoHead(struct List* list, const char* value);

/*функция для печати значений списка*/
void print(const char* str, size_t index);

/*печать списка*/
void printList(struct List *list, void(*print)(const char*, size_t));
