# include  "stdafx.h"
# include "create.h"
# pragma warning(disable:4996)

const size = 20;

/*стандартна€ ф-€ из  ормена*/
int HashU(const char *v, int size)
{
	int h, a = 31415, b = 27183;
	for (h = 0; *v != '\0'; v++, a = a*b % (size - 1))
		h = (a*h + *v) % size;
	return h;
}

/*создаЄт таблицу*/
struct List* createTable(int size)
{
	struct List* arr = (struct List*)malloc(size * sizeof(struct List));
	for (int j = 0; j < size; ++j)
	{
		arr[j] = *createList();
	}
	return arr;
}

/*добавление в таблицу*/
struct List* Insert(struct List* arr, int size)
{
	FILE* f = fopen("input.txt", "r");
	char c;
	while ((c = getc(f)) != EOF)
	{
		int length = 10;
		char * s = (char*)malloc(length * sizeof(char));
		int i = 0;
		while ((c != '\n') && (c != ' ') && (c != EOF))
		{
			s[i] = c;
			++i;
			if (i == length - 2)
			{
				s = realloc(s, length * 2);
				length *= 2;
			}
			c = getc(f);
		}
		s[i] = '\0';
		int index = HashU(s, size);
		InsertIntoHead(&(arr[index]), s);
	}
	return arr;
}

/*поиск*/
struct Node* FindInTable(struct List* arr, const char* key, int size, int (HashU)(const char*, int))
{
	int j = HashU(key, size);
	printf("%s ", (&(arr[j]))->head->value);
	struct Node* tmp = findInto((&(arr[j])), key);
	return tmp;
}

/*удаление элемента из таблицы*/
void DeleteElement(struct List* arr, const char* key, int size)
{
	int j = HashU(key, size);
	DeleteFromList((&(arr[j])), key);
}

/*очищение таблицы*/
void DeleteTable(struct List* arr, int size)
{
	struct List** arr1 = &arr;
	for (int j = 0; j < size; ++j)
	{
		if (&(arr[j]) != 0)
		{
			deleteList(((arr1)));
		}
	}
	free(arr);
}

/*распечатывает таблицу*/
void printTable(struct List* arr, int size)
{
	for (int j = 0; j < size; ++j)
	{
		printf("[%d] ", j);
		int i = 1;
		printList((&(arr[j])), print);
		printf("\n");
	}
}

void TestFuncTable()
{
	struct List* arr = createTable(20);
	arr = Insert(arr, size);
	printTable(arr, size);
	struct Node* tmp = FindInTable(arr, "Andrey", size, HashU);
	printf("%s \n", tmp->value);
	DeleteElement(arr, "Egor", size);
	printTable(arr, size);
	DeleteTable(arr, size);
	return;
}
 
int main()
{
	//TestFunc();
	TestFuncTable();
	getchar();
	return 0;
}



//int hash(char *v, int M){
//	int h = 0, a = 127;
//	for (; *v != '\0'; ++v)
//		h = (a*h + *v) % M;
//	return h;
//}

