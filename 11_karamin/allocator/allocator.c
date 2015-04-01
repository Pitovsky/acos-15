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
void PrintBlockInfo(Block* block)
{
	printf("Adress == %d\n", block);
	printf("Block->next address == %d\n", block->next);
	printf("Block->prev address == %d\n", block->prev);
	printf("Memory Adress == %d\n", block->Memory);
	printf("Occupied == %d\n", block->Occupied);
	printf("Size == %d\n", block->Size);
}
int printlist(void* p)
{
	int i = 0;
	list* lst = (list*)p;
	Block* block = lst->head;
	printf("_______________________\n");
	while (block)
	{
		i++;
		printf("Number == %d\n", i);
		PrintBlockInfo(block);
		printf("_______________________\n");
		block = block->next;
	}
	printf("NULL\n");
	printf("************************printing list finished******************************\n");
	return i;
}
int CountBlocks(list* List)
{
	Block* block = List->head;
	int i = 0;
	while (block)
	{
		i++;
		block = block->next;
	}
	return i;
}
void MergeBlocks(Block* block)
{
	Block* temp;
	temp = block->next;
	block->next = temp->next;
	if (temp->next) temp->next->prev = block;
	block->Size += sizeof(Block) + temp->Size;
}
void UniteFreeMemory(void* memory)
{
	list* List = (list*)memory;
	Block* block = List->head;
	while (block && block->next)
	{
		if (!(block->Occupied) && !(block->next->Occupied))
		{
			MergeBlocks(block);
			block = List->head;	
			continue;
		}
		block = block->next;
	}
}
void* CreateNewBlock(Block* tail)
{
	Block* temp = sbrk(MB);
	if (!temp) return NULL;
	tail->next = temp;
	
	temp->prev = tail;
	temp->next = NULL;
	temp->Memory = (void*)temp + sizeof(Block);
	temp->Occupied = 0;
	temp->Size = MB - sizeof(Block);
	return temp;
}
void* InitializeMemory(void **mem)
{
	void *p;
		
	p = sbrk(MB);										//Выделяем память
		
	list* memory = (list*)p;							//Указатель на двусвязный список блоков
	Block* head = (Block*)(p + sizeof(list));			//Указатель на первый головной блок
		
//Инициализация двусвязного списка
	memory->NumOfBlocks = 1;
	memory->head = head;
	memory->tail = head;
//Конец
		
//Инициализация первого блока памяти
	head->next = NULL;
	head->prev = NULL;
	head->Memory = (void*)head + sizeof(Block);
	head->Occupied = 0;
	head->Size = MB - sizeof(list) - sizeof(Block);
//Конец
	
	printf("Initializing:\n");
	printf("List address(sbrk) == %d\n", memory);
	printf("Head address == %d\n", head);
	printf("Head->memory == %d\n", head->Memory);
	printf("Head->size == %d\n", head->Size);
	
	*mem = p;
	return p;
}
Block* SplitBlock(Block* curr, size_t bytes)
{
	if (curr->Size < bytes + sizeof(Block)) 	//Если памяти в старом блоке слишком мало
	{
		printf("Not enough memory for split :(\n");
		return NULL;							//для нового, то блок не делить
	}
	if (curr->Size == bytes + sizeof(Block))
	{
		printf("Not enough memory for split but enough for data.\n");
		return curr;
	}
		
	Block* new 	= curr->Memory + bytes; 		//Засунуть новый блок после выделенной памяти
	new->next 	= curr->next;
	new->prev 	= curr;
	new->Memory = (void*)new + sizeof(Block);
	new->Occupied = 0;
	new->Size 	= curr->Size - sizeof(Block) - bytes;
	
	curr->next = new;	//Следующий это новый
	curr->Size = bytes;	//Размер текущего, уменьшенного до нужного размера блока становится равным bytes
	
	return new;
}
Block* FindAndSplitFreeBlock(list* mem, size_t bytes)
{
	Block* block;
	block = mem -> head;
	while (block != NULL)
	{
		if ((block->Size >= bytes) && (!block->Occupied))
		{
			if (SplitBlock(block, bytes))
				return block;
			else
				return NULL;
		}
		block = block->next;
	}
	return NULL;
}
void my_free(void* memory)
{
	Block* block = memory - sizeof(Block);
	block->Occupied = 0;
	while (block->prev) 
		block = block->prev;					//Пришли в голову списка
	memory = (void*)block - sizeof(list);		//В порядке, проверено
	UniteFreeMemory(memory);
	//printlist(memory);
}
void* my_malloc(unsigned int bytes)
{
	void* result;
	static void* p = NULL;				//Будет указателем на начало выделенного куска
					
	if (p == NULL)						//Если вдруг память раньше не выделялась
		InitializeMemory(&p);			//Проинициализировать память
		

	list* mem = p;						//Указатель на двусвязный список
	
	Block* GoodBlock = NULL;
	GoodBlock = FindAndSplitFreeBlock(mem, bytes);
	if (GoodBlock)									//Если есть подходящий блок
	{
		GoodBlock->Occupied = 1;
		result = (void*)GoodBlock->Memory;
	}
	else
	{
		void* ok = mem;
		while(ok && !GoodBlock)			//Выделяет пользователю память до тех пор пока ему не хватит
		{								//Ну или уже система не откажет в выделении
			ok = CreateNewBlock(mem->tail);
			if (ok) mem->tail = ok;
			UniteFreeMemory(mem);
			GoodBlock = FindAndSplitFreeBlock(mem, bytes);
			result = GoodBlock;					
		}
	}
	
	mem->NumOfBlocks = CountBlocks(mem);
	return result;
}

int main()
{
	void* List;
	
/*Вспомогательная информация*/
	printf("\nsizeof(Block) == %d\n", sizeof(Block));
	printf("sizeof(Block*) == %d\n", sizeof(Block*));
	printf("sizeof(list) == %d\n", sizeof(list));
	printf("sizeof(list*) == %d\n", sizeof(list*));
	printf("\n\n");
/***********************************************************************/	
/*Первый malloc*/
	int n;
	scanf("%d", &n);
	printf("Required Memory == %d\n\n", n*(int)sizeof(int));
	int* z = (int*)my_malloc(n*sizeof(int));	
	List = (void*)z - sizeof(Block) - sizeof(list);				//Здесь находится двусвязный список
	if (z == NULL)
	{
		printf("z == NULL");
		return 0;
	}
	z[0] = 99999;
	printf("Printf z == %d, OK\n", z);
/***********************************************************************/
/*Второй malloc*/
	scanf("%d", &n);
	printf("Required Memory == %d\n", n*(int)sizeof(int));
	int* q = (int*)my_malloc(n*sizeof(int));
	if (q == NULL)
	{
		printf("q == NULL");
		return 0;
	}
	q[0] = 898989;
	printf("Printf q == %d, OK\n", q);
/***********************************************************************/
/*Freing memory*/
	my_free(z);
	my_free(q);
/***********************************************************************/
/*Третий malloc*/
	scanf("%d", &n);
	printf("Required Memory == %d\n", n*(int)sizeof(int));
	int* w = (int*)my_malloc(n*sizeof(int));
	if (w == NULL)
	{
		printf("w == NULL");
		return 0;
	}
	w[0] = 898989;
/***********************************************************************/
/*Freing*/
	my_free(w);
/***********************************************************************/
	
	printlist(List);
	return 0;
}
