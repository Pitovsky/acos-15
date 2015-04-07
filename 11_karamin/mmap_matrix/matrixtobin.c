#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Wrong input.\n");
		return 1;
	}
	
	FILE *init, *out;
	init = fopen(argv[1], "r");
	if (init == NULL)
	{
		printf("Error opening init.txt");
		return 1;
	}
	
	out = fopen("matrix.txt", "wb");
	
	double* initm;
	int stlb, strk, i, j;
	fscanf(init, "%d%d", &stlb, &strk);
	initm = (double*)malloc(stlb*sizeof(double));
	
	for (i = 0; i < strk; ++i)
	{
		for(j = 0; j < stlb; ++j)
		{
			fscanf(init, "%lf", &initm[j]);
//			printf("%lf ", initm[j]);
		}
		fwrite(initm, sizeof(double), stlb, out);	
//		printf("\n");	
	}
	fclose(out);
	fclose(init);

	return 0;
}
