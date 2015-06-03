#include <stdio.h>
#include <unistd.h>
typedef struct list list;

char* bheap = 0;

struct list
{
	
	char ;
	int size;
	list* prev;
	list* next;
};

void printthis()
{
	list* tl = (list*) bheap;
	if (tl == 0)
	{
		printf("Элементов ещё нет\n");
	}
	while (tl != 0)
	{
		printf("%p  %d   %d\n", ((char*) tl) + sizeof(list), tl->size, tl->flag);
		tl = tl->next;
	}
	printf("\n");
};

void* my_malloc(int size)
{
	printf("Будет выделено %d единиц памяти\n", size);
	if (bheap==0) //если мы выделяем память первый раз
	{
		bheap = sbrk(1<<20);
		list a;	
		a.prev = 0;
		a.next = 0;
		a.size = (1<<20) - sizeof(list);
		a.flag = 0;
		list* bheap_n = (list*) bheap;
		*bheap_n = a;
	}
	list* stream = (list*) bheap; 
	list* stream_copy = stream;
	int key = 0;
	while ((key == 0) && (stream_copy != 0))
	{
		if ((stream_copy->size == size) && (stream_copy->flag == 0))
		{
			key = 1;
		}
		stream_copy = stream_copy->next;
	}
	if (key == 1)
	{
		stream_copy = stream_copy->prev;
		stream_copy->flag = 1;
		printthis();
		return ((char*) stream_copy) + sizeof(list);
	}
	else
	{
		while (((stream->size < size + sizeof(list)) && (stream->flag == 0) && (stream->next != 0)) || (stream->flag == 1))
		{
			stream = stream->next;
		}
		if (size >= stream->size + sizeof(list))
		{
			sbrk((1<<20) * ((size+sizeof(list)) / (1<<20) + 1));
		}
		char* last = (char*) stream;
		last += sizeof(list) + size;
		list* new_last = (list*) last;

		new_last->prev = stream;
		new_last->next = stream->next;
		new_last->flag = 0;
		new_last->size = stream->size - size - sizeof(list);

		stream->flag = 1;
		stream->size = size;
		stream->next = new_last;
		printthis();
		return (char*) stream + sizeof(list);
		}
}

void my_free(void* address)
{
	printf("Удаление элемента %p\n", address);
	list* addr = (list*) ((char*) address - sizeof(list));
	addr->flag = 0;
	if ((addr->next != 0) && (addr->next->flag == 0))
	{
		addr->size += addr->next->size + sizeof(list);
		addr->next = addr->next->next;
		if (addr->next != 0)
		{
			addr->next->prev = addr;
		}
	}
	if ((addr->prev != 0) && (addr->prev->flag == 0))
	{
		addr->prev->size += addr->size + sizeof(list);
		addr->prev->next = addr->next;
		if (addr->next != 0)
		{
			addr->next->prev = addr->prev;
		}
	}
	printthis();
};


int main()
{
    return 0;
}


