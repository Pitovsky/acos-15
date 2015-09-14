#include <unistd.h>
#include <stdio.h>

static const size_t mem_size = 1024*1024;

typedef struct Node
{
	struct Node* next;
	struct Node* prev;
	void* mem;
	size_t capacity;
	short free;  /* 1 - свободно; 0 - занято;*/
} Node;

typedef struct List
{
	Node * head;
	Node* tail;
} list;

Node* merge_free_blocks(Node* block, Node* next_block)
{
	block->capacity += sizeof(Node) + next_block->capacity;
	if (next_block->next)
	{
		block->next = next_block->next;
		next_block->next->prev = block;
	}

	else
	{
		block->next = NULL;
	}
	return block;
}

void split_mem(Node* tmp, size_t size)
{
	tmp->mem = (void*)tmp + sizeof(Node);
	Node* split_block = (void*)tmp + sizeof(Node) + size;


	tmp->free = 0; /*занят*/
	split_block->next = tmp->next;
	split_block->prev  = tmp;
	split_block->free = 1; /*свободен*/

	if (tmp->next != NULL)
	{
		tmp->next->prev = split_block;
		//split_block->next = tmp->next;
	}
	tmp->next = split_block;
	tmp->free = 1;

	split_block->capacity  = tmp->capacity  - sizeof(split_block) - size;
	tmp->capacity = size;
}

void* my_malloc(size_t size)
{
	list* mylist;
	static void* mem_point = NULL;
	if (mem_point == NULL)
	{
		mem_point = sbrk(mem_size);
		mylist = mem_point;
		mylist->head = (void*)mylist + sizeof(list);

		mylist->head->next = NULL;
		mylist->head->prev = NULL;

		mylist->tail = mylist->head;

		mylist->head->capacity  = mem_size - sizeof(list) - sizeof(Node);

		mylist->head->mem = (void*)mylist + sizeof(list) + sizeof(Node);
		mylist->head->free = 1;
		//printf("tail addr: %d \n", )
		//printf("mem address : %d\n ", mem_point);
		//printf("sizeof(Node)  + sizeof(list): %d \n", sizeof(list) + sizeof(Node));
	}

	mylist = mem_point;
	Node* temp = mylist->head;

	while (temp)
	{
		if ((temp->capacity  >= size) && (temp->free))
		{
			split_mem(temp, size);
			return temp->mem;
		}
		temp = temp->next;
	}
	return NULL;
}

void my_free(void* point)
{
	Node* tmp = point - sizeof(Node);
	tmp->free = 1;


	while (((tmp->next)&&(tmp->next->free))||((tmp->prev)&&(tmp->prev->free)))
	{
		if (tmp->next)
		{
			if (tmp->next->free)
			{
				tmp = merge_free_blocks(tmp, tmp->next);
			}
		}
		if (tmp->prev)
		{
			if (tmp->prev->free)
			{
				tmp = merge_free_blocks(tmp->prev, tmp);
			}
		}
	}
}

int main()
{
	int i;

	int * a = (int*)my_malloc(5* sizeof(int));
	a[0] = 5;
	printf("%d \n", a[0]);
	printf("in main mas a point: %d\n\n", a);
	//my_free(a);

	char * b = (char*)my_malloc(5 * sizeof(char));
	b[4] = 'a';
	printf("%c \n", b[4]);
	printf("in main mas b point: %d\n\n", b);
	my_free(b);

	//printf("There is no error\n");
	char** c = (char**)my_malloc(6*sizeof(char*));
    //printf("There is no error\n");
	for ( i = 0; i < 6; ++i)
	{
        //printf("There is no error\n");
		c[i] = (char*)my_malloc(3*sizeof(char));
	}
	printf("There is no error\n");
	//printf("There is no error\n");
	c[0][0] = '0';
	c[4][4] = '4';
	printf("double mas test: %c\n", c[0][0]);
	printf("double mas test: %c\n", c[4][4]);
	printf("in main mas c point: %d\n\n", c);

	int * d = (int*)malloc(5 * sizeof(int));


	my_free(a);
	return 0;
}
