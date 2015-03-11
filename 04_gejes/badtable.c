#include <stdio.h>
#include <string.h>

struct List
{
	struct List* next;
	int value;
	char* key;
};
struct Table
{
	size_t size;
	struct List **korts;

};

int hash(char* word, int size)
{
		int i = 0;
		int sum = 0;
		for (i = 0; i < strlen(word); ++i)
			sum = sum + (int)word[i];
		return sum%size;
}

struct Table* createTable(int newSize)
{
	struct Table* newTable = (struct Table*)malloc(sizeof(struct Table));
	newTable->size = newSize;
	newTable->korts = (struct List**)malloc(newSize*sizeof(struct List*));
	return newTable;
}
void clearTable(struct Table *p)
{
    int i = 0;
    for (i = 0; i < p->size; ++i)
        if (p->korts[i] != NULL)
        {
            struct List *nextPage = p->korts[i];
            while (nextPage != NULL)
            {
                free(nextPage->key);
                nextPage = nextPage->next;
            }
            free(p->korts[i]);
        }
    free(p->korts);
}
void outTable(struct Table *p)
{
    int i = 0;
    struct List* nextPage = NULL;
    for (i = 0; i < p->size; ++i)
    {
        printf("%d:", i);
        if (p->korts[i] != NULL)
        {
            nextPage = p->korts[i];
            while (nextPage != NULL)
            {
                printf(" -> [%s|%d]", nextPage->key, nextPage->value);
                nextPage = nextPage->next;
            }
        }
        printf("\n");
    }
}

void writeElement(struct Table *p, char *key, int value)
{
    char *newKey = (char*)malloc(strlen(key)*sizeof(char));
    int i = 0;
    for (i = 0; i < strlen(key); ++i)
        newKey[i] = key[i];
	if (p->korts[hash(key, p->size)] == NULL)
	{
        p->korts[hash(key, p->size)] = (struct List*)malloc(sizeof(struct List));
        p->korts[hash(key, p->size)]->next = NULL;
        p->korts[hash(key, p->size)]->value = value;
        p->korts[hash(key, p->size)]->key = newKey;
        return;
	}
	struct List *nextPage = p->korts[hash(key, p->size)];
	while (nextPage->next != NULL)
		nextPage = nextPage->next;
	nextPage->next = (struct List*)malloc(sizeof(struct List));
	nextPage->next->next = NULL;
	nextPage->next->value = value;
	nextPage->next->key = newKey;
}
void deleteElement(struct Table *p, char *key)
{
	struct List *nextPage = p->korts[hash(key, p->size)];
	if (p->korts[hash(key, p->size)] == NULL)
        return;
	if (p->korts[hash(key, p->size)]->next == NULL)
	{
		free(p->korts[hash(key, p->size)]);
		p->korts[hash(key, p->size)] = NULL;
		return;
	}
	if (strcmp(nextPage->key, key) == 0)
    {
        struct List *afterDelPage = nextPage->next;
        free(nextPage->key);
        p->korts[hash(key, p->size)] = afterDelPage;
        return;
    }
	while (nextPage->next != NULL && strcmp(nextPage->next->key, key) != 0)
		nextPage = nextPage->next;
    if (nextPage->next == NULL)
        return;
	struct List *afterDelPage = nextPage->next->next;
	free(nextPage->next->key);
	free(nextPage->next);
	nextPage->next = afterDelPage;
}
int readElement(struct Table *p, char *key, int *dst)
{
	struct List *nextPage = p->korts[hash(key, p->size)];
	while (nextPage != NULL && strcmp(nextPage->key, key) != 0)
		nextPage = nextPage->next;
	if (nextPage == NULL)
		return 0;
	*dst = nextPage->value;
	return 1;
}

int main()
{
	struct Table *ptr;
	int n = 0;
	int v = 0;
	scanf("%d", &n);
	ptr = createTable(n);
	char com[3];
	char keys[10];
	while (strcmp(com, "end") != 0)
	{
        scanf("%s", com);
        if (strcmp(com, "add") == 0)
        {
            scanf("%s", keys);
            scanf("%d", &v);
            writeElement(ptr, keys, v);
        }
        else if (strcmp(com, "get") == 0)
        {
            scanf("%s", keys);
            if (readElement(ptr, keys, &v))
                printf("out: %s <=> %d\n", keys, v);
            else
                printf("I haven't it\n");
        }
        else if (strcmp(com, "pop") == 0)
        {
            scanf("%s", keys);
            deleteElement(ptr, keys);
            printf("Ok.\n");
        }
        else if (strcmp(com, "out") == 0)
            outTable(ptr);
	}

	clearTable(ptr);

	return 0;
}
