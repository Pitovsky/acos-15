#include <stdio.h>
#include <time.h>
#include "header.h"
//#include "ctime"
#include <stdlib.h>

int main()
{
	int i, j;
	int num_col, num_str;
	scanf("%d", &num_col);
	scanf("%d", &num_str);
	
	srand(time(0));
	
	
	int fd = open("matr.bin", 
				  O_RDWR|O_CREAT,
				  S_IRWXU);
				  
	FILE* test = fopen("matr.txt", "wb");
				  
	double buf[num_col * num_str];
	
   for (i = 0; i <	num_str; ++i)
	{
		for (j = 0; j < num_col; ++j)
		{
			//printf("there is no error\n");
			double elem = 0 +  ((rand() % 10000) * 1.0) / 57;
			buf[i * num_col + j] = elem;
			write(fd, &buf[i * num_col + j], sizeof(double));
			fprintf(test, "%lf\t", elem);
		}
		fprintf(test, "\n");
	}

	//printf("%lf ", elem);
	close(fd);
	fclose(test);
				  
	return 0;
}
