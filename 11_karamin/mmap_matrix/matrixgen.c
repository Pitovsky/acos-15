#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main(int argc, char** argv)
{
	if (argc < 4)
	{
		printf("Wrong input. (file rows cols)\n");
		return 1;
	}
	FILE *out = fopen(argv[1], "w");
	srand(time(NULL));
	int rows = atoi(argv[2]), cols = atoi(argv[3]), i, j;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			fprintf(out, "%lf ", (double)(rand() % 1000000)/10000); 
		}
		fprintf(out, "\n");
	}
	
	fclose(out);
	return 0;
}
