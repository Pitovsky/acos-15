#include <stdio.h>
#include <string.h>

struct List
{
        struct List* next;
        void* value;
        char* key;
};
struct Table
{
        size_t size;
        struct List **korts;
    size_t realSize;
};

int hash(char* word, int size)
{
                int i = 0;
                int sum = 0;
                int len = strlen(word);
                for (i = 0; i < len; ++i)
                        sum = sum + (int)word[i];
                return sum%size;
}

struct Table* createTable(int newSize)
{
        struct Table* newTable = (struct Table*)malloc(sizeof(struct Table));
        newTable->size = newSize;
        newTable->realSize = 0;
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
void outTable(struct Table *p, FILE* fout)
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
                fprintf(fout, " -> [%s|%d]", nextPage->key, *((int*)(nextPage->value)));
                nextPage = nextPage->next;
            }
        }
        printf("\n");
    }
}

struct Table *writeElement(struct Table *p, char *key, void* value)
{
    char *newKey = (char*)malloc(strlen(key)*sizeof(char));
    int i = 0;
    strcpy(newKey, key);
        if (p->korts[hash(key, p->size)] == NULL)
        {
        p->korts[hash(key, p->size)] = (struct List*)malloc(sizeof(struct List));
        p->korts[hash(key, p->size)]->next = NULL;
        void* newv = (void*)malloc(sizeof(*value));
        memcpy(newv, value, sizeof(*value));
        p->korts[hash(key, p->size)]->value = newv;
        p->korts[hash(key, p->size)]->key = newKey;
        }
        else
        {
        struct List *nextPage = p->korts[hash(key, p->size)];
        while (nextPage->next != NULL)
            nextPage = nextPage->next;
        nextPage->next = (struct List*)malloc(sizeof(struct List));
        nextPage->next->next = NULL;
        void* newv = (void*)malloc(sizeof(*value));
        memcpy(newv, value, sizeof(*value));
        nextPage->next->value = newv;
        nextPage->next->key = newKey;
    }
    ++p->realSize;
    if (p->realSize > 4*p->size/3)
    {
        struct Table *pnew = createTable(2*p->size);
        struct List *nextPage;
        for (i = 0; i < p->size; ++i)
        {
            nextPage = p->korts[i];
            while (nextPage != NULL)
            {
                writeElement(pnew, nextPage->key, nextPage->value);
                nextPage = nextPage->next;
            }
        }
        clearTable(p);
        return pnew;
    }
    return p;
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
        free(nextPage->value);
        p->korts[hash(key, p->size)] = afterDelPage;
        return;
    }
        while (nextPage->next != NULL && strcmp(nextPage->next->key, key) != 0)
                nextPage = nextPage->next;
    if (nextPage->next == NULL)
        return;
        struct List *afterDelPage = nextPage->next->next;
        free(nextPage->next->key);
        free(nextPage->next->value);
        free(nextPage->next);
        nextPage->next = afterDelPage;
}
int readElement(struct Table *p, char *key, void** dst)
{
        struct List *nextPage = p->korts[hash(key, p->size)];
        while (nextPage != NULL && strcmp(nextPage->key, key) != 0)
                nextPage = nextPage->next;
        if (nextPage == NULL)
                return 0;
        *dst = nextPage->value;
        return 1;
}
