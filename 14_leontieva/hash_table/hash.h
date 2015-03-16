
typedef struct node node;
node ** create(int M);
unsigned int hash(char* key, int M);
void insert1(char* key, node** T, int M);
node* search_by_key(char* key,node** T, int M);
void delete(char* key, node** T, int M);
void show_node(char* key, node** T, int M);
