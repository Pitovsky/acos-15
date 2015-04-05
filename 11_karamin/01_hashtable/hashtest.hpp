int hashtest()
{
	int number;
	printf("Enter testfile number (test1-4) --> ");
	scanf("%d", &number);

	FILE *in, *out, *inputfile;
	char* filepath = (char*)malloc(MAX_FILE_NAME*sizeof(char));
	sprintf(filepath, "HashTable/test%d.txt", number);
	in = fopen(filepath, "r");
	out = fopen("HashTable/out.txt", "r");
	inputfile = fopen("HashTable/in.txt", "w");
	if (in == NULL)
		return 1;

	int tsize;
	printf("\nEnter table size --> ");
	scanf("%d", &tsize);

	//Entering Data with a key from file
	int count, i, finish;
	fscanf(in, "%d", &finish);
	struct data** input = (struct data**)malloc(finish*sizeof(struct data*));
	for (i = 0; i < finish; ++i){
		input[i] = (struct data*)malloc(MaxStringSize*sizeof(struct data));
		input[i]->KEY = (char*)malloc(MaxStringSize*sizeof(char));
		fscanf(in, "%s%d", input[i]->KEY, &(input[i]->my_data));
	}
	count = i;
	//C-Strings


	//Inserting data in the table
	struct table* h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		InsertInTheTable(h, input[i]);
	printf("\nTable created and filled from test%d.", number);
	PrintTable(h);
	//Inserting data in the table


	//Inserting in the table from keyboard
	struct data *ins = (struct data*)malloc(sizeof(struct data));
	printf("\n\nEnter data to insert:\n");
	key KEY = (key)malloc(MaxStringSize*sizeof(char));
	int data;
	printf("Enter key --> ");
	scanf("%s", KEY);
	printf("\nEnter data(int) --> ");
	scanf("%d", &data);

	ins->KEY = KEY;
	ins->my_data = data;
	InsertInTheTable(h, ins);

	printf("\n\n");
	PrintTable(h);
	//Inserting in the table from keyboard


	//Finding in the table
	struct data* p;
	key Key = (key)malloc(MaxStringSize*sizeof(char));
	printf("\nEnter key to find data --> ");
	scanf("%s", Key);
	p = FindInTheTable(h, Key);
	printf("Finded data --> ");
	if (p == NULL) printf("NULL");
	else printf("%d", p->my_data);
	//Finding in the table


	//Printing data by hash
	int hash;
	printf("\n\nEnter HASH(int) to print data -->");
	scanf("%d", &hash);
	PrintList(h->mytable[hash]);
	//Printing data by hash


	//Deleting element from table
	key del = (key)malloc(100 * sizeof(char));
	printf("\n\nEnter deleting string --> ");
	scanf("%s", del);
	DeleteFromTable(h, del);

	printf("\n\nModified table:\n");
	PrintTable(h);
	//Deleting element from table


	//Clearing and deleting table
	ClearTable(h);
	printf("\nTable cleared!\n");
	PrintTable(h);
	DeleteTable(h);
	//Clearing and deleting table

	printf("\nAll good.\n");

	return 0;
}
