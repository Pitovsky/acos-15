typedef char* key;

struct table
{
	int Size;
	int NumberOfElements;
	struct node** mytable;
};
struct node{
	struct node* next;
	struct node* previous;
	struct data* input;
};
struct data{
	key KEY;
	int data;
};

int HashU(char* s, int M);

struct table* CreateTable(int tsize);
int InsertInTheTable(struct table* Table, struct data* data);
struct data* FindInTheTable(struct table* table, key KEY);
int DeleteFromTable(struct table* table, key KEY);

void PrintTable(struct table* table);
void PrintList(struct node* list);
int HashU(char* s, int M);

void ClearTable(struct table* table);
void DeleteTable(struct table* table);