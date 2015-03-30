#include <stdio.h>
#include <unistd.h>

#define MB 1024*1024

int copy(void* from, void* where, size_t bytes)
{
	if (bytes == 0 || from == NULL || where == NULL) return 1;
	
	int i; char *From, *Where;
	From = (char*)from;
	Where = (char*)where;
	for (i = 0; i <= bytes; ++i)
	{
		 *(Where+i) = *(From+i);
	}
	return 0;
}

typedef struct Block
{
	struct Block* next;
	struct Block* prev;
	void* Memory;
	int Occupied;
	int Size;
} Block;

typedef struct list
{
	struct Block* head;
	struct Block* tail;
	int NumOfBlocks;
} list;

void* InitializeMemory(void **Mem)
{
	printf("initialize memory started\n");
		void *p = &Mem;
		
		//list Memory;					//Создаем двусвязный список
		//Block Head;						//Создаем начало всем блокам
		p = sbrk(MB);					//Выделяем память
		
		//copy(&Memory, p, sizeof(list));//Вставка в выделенную память двусвязного списка
		//copy(&Head, p + sizeof(list) + 1, sizeof(Block)); //Первый элемент этого списка
		
		list* memory;					//Указатель на двусвязный список блоков
		Block* head;				//Указатель на первый головной блок
		
		memory = (list*)p;							//Указатель на двусвязный список блоков
		head = (Block*)(p + sizeof(list) + 1);		//Указатель на первый головной блок
		
//Инициализация двусвязного списка
		memory->NumOfBlocks = 1;
		memory->head = head;
		memory->tail = head;
//Конец
		
//Инициализация первого блока памяти
		head->next = NULL;
		head->prev = NULL;
		head->Memory = (void*)(p + sizeof(list) + sizeof(Block) + 1);
		head->Occupied = 0;
		head->Size = MB - sizeof(list) - sizeof(Block);
//Конец
	printf("initialize memory finished\n");
	return p;
}

Block* SplitBlock(Block* curr, size_t bytes)
{
	if (curr->Size <= bytes + sizeof(Block)) 	//Если памяти в старом блоке слишком мало
		return NULL;							//для нового, то блок не делить
		
	Block* new 	= curr->Memory + bytes; 		//Засунуть новый блок после выделенной памяти
	new->next 	= curr->next;
	new->prev 	= curr;
	new->Memory = 
	return NULL;
}

Block* FindFreeBlock(list* mem, size_t bytes)
{
	Block* block;
	block = mem -> head;
	
	while (block != NULL)
	{
		if ((block->Size >= bytes) && (!block->Occupied)) return block;
	}
	return NULL;
}

void* my_malloc(unsigned int bytes)
{
	void* result;
	static void* p = NULL;				//Будет указателем на начало выделенного куска
					
	if (p == NULL)						//Если вдруг память раньше не выделялась
		InitializeMemory(&p);			//Проинициализировать память

	list* mem = p;						//Указатель на двусвязный список
	
	if (bytes <= MB - sizeof(list) - mem->NumOfBlocks*sizeof(Block))
	{
		Block* GoodBlock = FindFreeBlock(mem, bytes);
		if (GoodBlock)									//Если есть подходящий блок
		{
			Block* new = SplitBlock(GoodBlock, bytes);
			if (new != NULL)
				result = (void*)new->Memory;
			else
				result = NULL;
		}
		else //Если подходящего блока нет, то по сути нужно выделить еще sbrk
		{
			result = NULL;
		}



/*		while(block != NULL)
		{
			
			if(block->Size >= bytes && !block->Occupied) //Где-то здесь перераспределить память по блоку
			{
				block->Occupied = 1;
				//Block temp;
				Block *new, *curr;
				//copy(&temp, block->Memory + 1, sizeof(Block));
				curr = block;
				
				new = (Block*)block->Memory + bytes;  
//Вставка нового элемента в двусвязный список
				new->Occupied = 0;
				new->next = curr->next;
				new->prev = curr;
				new->Size = curr->Size - sizeof(block) - bytes;
				curr->next = new;
				
				mem->NumOfBlocks++;
//Конец
				result = block->Memory;
			}

			block = block->next;
		}*/
		result = NULL; 				//Если не нашли подходящий по памяти блок
	}
	else
	{
	//Если памяти недостаточно реализовать позже
	}
	
	
	return result;
}

int main()
{
	int n, i;
	scanf("%d", &n);
	printf("scanf OK\n");
	int* z = (int*)my_malloc(n*sizeof(int));
	//int *z = (int*)123456;
	printf("Mymalloc OK?\n");
	if (z == NULL)
	{
		printf("z == NULL");
		return 0;
	}
	printf("%d\n", z[0]);
	//int* q = (int*)my_malloc(15*sizeof(int));
	//printf("Copy is OK\n");
	//printf("\ntest->next == %d\n", test->next);
	
	for (i = 0; i < n; ++i)
	{
		z[i] = (n-i)*100;
	}
	
	for(i = 0; i< n; ++i)
	{
		printf("z[%d] = %d\n", i, z[i]);
	}
	return 0;
}
