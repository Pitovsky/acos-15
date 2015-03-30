#include <stdio.h>
#include <unistd.h>

char* heap_begin = NULL;

typedef struct list list;
struct list
{
    int size;
    char flag;
    list* prev;
    list* next;
};

void print_list()
{
    list* help = (list*) heap_begin;
    if (help == NULL)
    {
        printf("no memory allocated\n");
    }
    while (help != NULL)
    {
        printf("%p  %d   %d\n", ((char*) help) + sizeof(list), help->size, help->flag);
        help = help->next;
    }
    printf("\n");
}

void* my_malloc(int size)
{
    printf("Выделение памяти размером %d...\n", size);
    if (heap_begin == NULL) //если мы выделяем память первый раз
    {
        heap_begin = sbrk(1<<20);
        list a;
        a.prev = NULL;
        a.next = NULL;
        a.size = (1<<20) - sizeof(list);
        a.flag = 0;
        list* heap_begin_new = (list*) heap_begin;
        *heap_begin_new = a;
    }
    list* stream = (list*) heap_begin; 
    list* stream_copy = stream;
    int key = 0;//key : 0 - нет уже выделенного свободного куска размера, совпадающего с требуемым ; 1 - такой кусок есть
    while ((key == 0) && (stream_copy != NULL))//поиск такого куска, если он найдется, то stream_copy будет указателем на следующий после него кусок (последний кусок(элемент списка) всегда свободен
    {
        if ((stream_copy->size == size) && (stream_copy->flag == 0))
        {
            key = 1;
        }
        stream_copy = stream_copy->next;
    }
    if (key == 1)//кусок ровно требемого размера нашелся
    {
        stream_copy = stream_copy->prev;
        stream_copy->flag = 1;
        print_list();
        return ((char*) stream_copy) + sizeof(list);
    }
    else//кусок ровно требуемого размера не нашелся
    {
        while (((stream->size < size + sizeof(list)) && (stream->flag == 0) && (stream->next != NULL)) || (stream->flag == 1))//доходим либо до последнего элемента, либо до элемента, в который поместится нужный нам размер + лист списка + хотя бы один байт
        {
            stream = stream->next;
        }
        if (size >= stream->size + sizeof(list))//условие может быть верно только у последнего элемента, поэтому если размера не хватает, то мы выделяем нужное количество мегабайт(кусков, на которые мы сдвигаем через sbrk
        {
            sbrk((1<<20) * ((size+sizeof(list)) / (1<<20) + 1));
        }
        char* last = (char*) stream;//создаем новый последний элемент списка(new_last)
        last += sizeof(list) + size;
        list* new_last = (list*) last;

        new_last->prev = stream;//заполняем его
        new_last->next = stream->next;
        new_last->flag = 0;
        new_last->size = stream->size - size - sizeof(list);

        stream->flag = 1;
        stream->size = size;
        stream->next = new_last;
        print_list();
        return (char*) stream + sizeof(list);
    }
}

void my_free(void* address)
{
    printf("Удаление элемента по адресу %p...\n", address);
    list* addr = (list*) ((char*) address - sizeof(list));
    addr->flag = 0;
    if ((addr->next != NULL) && (addr->next->flag == 0))
    {
        addr->size += addr->next->size + sizeof(list);
        addr->next = addr->next->next;
        if (addr->next != NULL)
        {
            addr->next->prev = addr;
        }
    }
    if ((addr->prev != NULL) && (addr->prev->flag == 0))
    {
        addr->prev->size += addr->size + sizeof(list);
        addr->prev->next = addr->next;
        if (addr->next != NULL)
        {
            addr->next->prev = addr->prev;
        }
    }
    print_list();
}

//void* my_realloc(void* a, int size)

int main()
{
    void *a1 = my_malloc(10);
    void *a2 = my_malloc(100);
    void *a3 = my_malloc(400);
    //void *a4 = my_malloc(12);
    my_free(a1);
    my_free(a3);
    my_free(a2);
    return 0;
}
