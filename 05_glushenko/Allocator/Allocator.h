#include <unistd.h>
#include <stdio.h>
#include <string.h>

static const unsigned MEMORY = 1 << 20;


typedef struct Page
{
    unsigned size;
    char free;
    struct Page *next, *prev;
} Page;

Page *CreatePage();
void *Request(unsigned Space);
Page *LastPage(Page *PageNum);
void *Allocate(Page *page, unsigned Space);
void MergeForward(Page *page);
void MergeBackward(Page *page);
Page *IncreasePage(Page *PageNum);
void *my_malloc(unsigned Space);
