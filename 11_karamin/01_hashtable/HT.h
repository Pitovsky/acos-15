typedef char* key;

struct node{
	struct node* next;
	struct node* previous;
	struct data* input;
};

struct data{
	key KEY;
	void* data;
};

int CompareStrings(char* string1, char* string2);
int FileReadString(FILE* f, char* s);
int ReadString(char* s);
struct node** CreateTable(int tsize);
void PrintTable(struct node** table, int tsize);
int HashU(char* s, int M);
int Insert(struct node** table, int tsize, struct data* datain);
int Delete(struct node** table, int tsize, key KEY);
void ClearTable(struct node** table, int tsize);
void DeleteTable(struct node** table, int tsize);