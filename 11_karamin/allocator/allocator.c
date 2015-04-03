#include <stdio.h>
#include <unistd.h>

#define MB 1024*1024												//Сколько выделяет за раз sbrk()

int min(int a, int b)
{
    return a<b?a:b;
}
int copy(void* from, void* where, size_t bytes)						//Скопировать bytes байт из from в where
{
    if (bytes == 0 || from == NULL || where == NULL) return 1;

    int i; char *From, *Where;
    From = (char*)from;
    Where = (char*)where;
    for (i = 0; i < bytes; ++i)
    {
         *(Where+i) = *(From+i);
    }
    return 0;
}
typedef struct Block
{
    struct Block* next;
    struct Block* prev;
    void* memory;
    int Occupied;
    size_t size;
} Block;
typedef struct list
{
    struct Block* head;
    int NumOfBlocks;
} list;
void PrintBlockInfo(Block* block)									//Вывести информацию о блоке
{
    printf("Adress == %d\n", block);
    printf("Block->next address == %d\n", block->next);
    printf("Block->prev address == %d\n", block->prev);
    printf("Memory Adress == %d\n", block->memory);
    printf("Occupied == %d\n", block->Occupied);
    printf("size == %d\n", block->size);
}
int printlist(void* p)												//Вывести весь двусвязный список
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
    printf("NULL\n\n");
    return i;
}
int CountBlocks(list* List)											//Посчитать количество блоков в списке
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
Block* GetTail_l(list* List)										//Вернуть конец списка
{
    Block* block;
    block = List->head;
    while (block->next)
        block = block->next;
    return block;
}
Block* GetHead(Block* anyblock)										//Вернуть начало списка из любого блока
{
    if (anyblock == NULL) return NULL;
    while (anyblock->prev)
        anyblock = anyblock->prev;
    return anyblock;
}
Block* GetTail(Block* anyblock)										//Вернуть конец списка
{
    if (anyblock == NULL) return NULL;
    while (anyblock->next)
        anyblock = anyblock->next;
    return anyblock;
}
list* GetList(Block* anyblock)										//Вернуть указатель на список из любого блока
{
    return (list*)((void*)GetHead(anyblock) - sizeof(list));
}
void MergeBlocks(Block* block)										//Слить блоки block и block->next
{
    Block* temp;
    temp = block->next;
    block->next = temp->next;
    if (temp->next) temp->next->prev = block;
    block->size += sizeof(Block) + temp->size;						//Объединить размеры
}
void UniteFreeMemory(list* memory)									//Объединить все свободные соседние куски памяти в списке
{
    list* List = memory;
    Block* block = List->head;
    while (block && block->next)									//Слияние идет если два соседних существуют
    {
        if (!(block->Occupied) && !(block->next->Occupied))			//И оба не заняты
        {
            MergeBlocks(block);										//Тогда сливаем их
            continue;
        }
        block = block->next;
     }
}
void* CreateNewBlock(Block* tail)									//Создать новый блок с помощью  sbrk();
{
    Block* temp = sbrk(MB);
    if (!temp) return NULL;

    tail->next = temp;												//Вставка блока в конец двусвязного списка

    temp->prev = tail;
    temp->next = NULL;
    temp->memory = (void*)temp + sizeof(Block);
    temp->Occupied = 0;
    temp->size = MB - sizeof(Block);

    return temp;
}
void* InitializeMemory(void **mem)									//Если память не выделялась, то выделить её и создать список
{
    void *p;

    p = sbrk(MB);													//Выделяем память

    list* memory = (list*)p;										//Указатель на двусвязный список блоков
    Block* head = (Block*)(p + sizeof(list));						//Указатель на первый головной блок

//Инициализация двусвязного списка
    memory->NumOfBlocks = 1;
    memory->head = head;
//Конец

//Инициализация первого блока памяти
    head->next = NULL;
    head->prev = NULL;
    head->memory = (void*)head + sizeof(Block);
    head->Occupied = 0;
    head->size = MB - sizeof(list) - sizeof(Block);
//Конец
/*
    printf("Initializing:\n");
    printf("List address(sbrk) == %d\n", memory);
    printf("Head address == %d\n", head);
    printf("Head->memory == %d\n", head->memory);
    printf("Head->size == %d\n", head->size);
*/
    *mem = p;
    return p;
}
Block* SplitBlock(Block* curr, size_t bytes)
{
    if (curr->size < bytes)											//Если памяти в старом блоке слишком мало, то не делить
    {
        printf("Not enough memory for split :(\n");
        return NULL;
    }
    if (curr->size >= bytes && curr->size <= bytes + sizeof(Block) && curr != GetTail(curr))
    {
        printf("Not enough memory for split but enough for data.\n");
        return curr;
    }

    Block* newblock = curr->memory + bytes;							//Засунуть новый блок после выделенной памяти
    newblock->next       = curr->next;
    newblock->prev       = curr;
    newblock->memory     = (void*)newblock + sizeof(Block);
    newblock->Occupied   = 0;
    newblock->size       = curr->size - sizeof(Block) - bytes;

    curr->next = newblock;											//Следующий это новый
    curr->size = bytes;												//Размер текущего, уменьшенного до нужного размера блока становится равным bytes

    return newblock;
}
Block* FindAndSplitFreeBlock(list* mem, size_t bytes)				//Найти блок доходящего размера и разделить его, вернуть указатель на него же, если успех, NULL иначе.
{
    Block* block;
    block = mem -> head;
    while (block != NULL)											//Проход по двусвязному списку в поиске подходящего блока
    {
        if ((block->size >= bytes) && (!block->Occupied))
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
void* my_realloc(void* mem, size_t newsize)
{
    Block* result;
    Block* block = mem - sizeof(Block);
    Block* temp, *tail = GetTail(block);
    list* List = GetList(block);
    block->Occupied = 0;
    temp = FindAndSplitFreeBlock(List, newsize);					//Найти подходящий блок, разделить его и вернуть пользователю
    if (temp)														//Если походящий найден, то информацию из старого куска памяти скопировать
    {
        copy(block->memory, temp->memory, min(block->size, newsize));
        result = temp;
        temp->Occupied = 1;
    }
    else															//Если подходящий не найден, выделяем память, пока пользователю не хватит
    {
        int i = 1;
        Block* newmem = CreateNewBlock(tail);
        while (newsize > MB*(i) && newmem)							//В цикле выделяем память, пока пользователю не хватит
        {
            newmem = CreateNewBlock(newmem);
            i++;
        }
        UniteFreeMemory(List);										//Ну и объединяем все выделенные блоки в один
        
		if (!newmem)
            return NULL;
																	//Если всё прошло успешно, в конце списка будет блок, подходящий по запросу памяти пользователем.
        result = FindAndSplitFreeBlock(List, newsize);				//Найти и разделить блок, в данном случае последний.
        result->Occupied = 1;
    }
    return result->memory;
}
void my_free(void* memory)											//Освобождение памяти
{
    Block* block = memory - sizeof(Block);
    block->Occupied = 0;											//Говорим, что блок свободен, и перераспределяем память между блоками
    
	memory = GetList(block);
    UniteFreeMemory(memory);										//Оптимизация свободной памяти
}
void* my_malloc(unsigned int bytes)
{
    void* result;
    static void* p = NULL;											//Будет указателем на начало выделенного куска

    if (p == NULL)													//Если вдруг память раньше не выделялась
        InitializeMemory(&p);										//Проинициализировать память

    list* mem = p;													//Указатель на созданный двусвязный список

    Block* GoodBlock = NULL;
	GoodBlock = FindAndSplitFreeBlock(mem, bytes);					//Ищем подходящий пользователю по памяти блок
    if (GoodBlock)													//Если есть подходящий блок
    {
        GoodBlock->Occupied = 1;
        result = (void*)GoodBlock->memory;							//Выделяем его пользователю
    }
    else															//Если же такого блока нет, то мы вынуждены выделять память
    {
        GoodBlock = GetTail_l(mem);
        Block* temp; 
		result = GoodBlock;											//Инициализация чем-нибудь, не важно
        int i = 0;
        while(i*MB + GoodBlock->size < bytes && result)				//Выделяем память до тех пор, пока ему не хватит
        {
            temp = CreateNewBlock(GetTail_l(mem));
            i++;
        }
        if (!temp) 
			result = NULL;											//Если не выделилась - значит система отказала в выделении
        else
        {
            UniteFreeMemory(mem);									//Если всё в порядке, то объединить все свободные соседние куски памяти
            temp = FindAndSplitFreeBlock(mem, bytes);				//Теперь памяти точно хватит
            
			if (temp)												//Если всё прошло успешно, отправить пользователю выделенную память
            {
                result = temp->memory;
                temp->Occupied = 1;
            }
            else													//Если что-то пошло не так. Возникать не должно.
                result = NULL;
        }
    }

    mem->NumOfBlocks = CountBlocks(mem);							//Ну и просто подсчет количества блоков памяти в двусвязном списке
    return result;
}

int main()
{
    void* List;
    int n;

/***********************************************************************/
/*Первый malloc*/
    scanf("%d", &n);
    char* a = (char*)my_malloc(n*sizeof(char));
    List = (void*)a - sizeof(Block) - sizeof(list);                        //Здесь находится двусвязный список
/***********************************************************************/
/*Второй malloc*/
    scanf("%d", &n);
    char* b = (char*)my_malloc(n*sizeof(char));
/***********************************************************************/
/*Третий malloc*/
    scanf("%d", &n);
    char* c = (char*)my_malloc(n*sizeof(char));
/***********************************************************************/
/*Четвёртый malloc*/
    scanf("%d", &n);
    char* d = (char*)my_malloc(n*sizeof(char));
/***********************************************************************/
	printlist(List);
    
    scanf("%d", &n);
    d = my_realloc(d, n);
 
    scanf("%d", &n);
    b = my_realloc(b, n);
 
	printlist(List);
    
    my_free(a);
    my_free(b);
    my_free(c);
    my_free(d);
	
	printlist(List);
    
/***********************************************************************/

/***********************************************************************/
    return 0;
}
