#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int* Number;
	if (argc < 3)
	{
		printf("wrong input");
		return 1;
	}
	int PrNum;
	sscanf(argv[argc-1], "%d", &PrNum);
	
	int i, *Pr = (int*)malloc((argc-2) * sizeof(int));
	Number = (int*)malloc(PrNum*sizeof(int));
	for (i=0; i< argc-2; ++i) Number[i] = 0;
	for (i = 0; i < PrNum; ++i)
	{
		Pr[i] = fork();
		if (Pr[i] == 0)
		{
			int j;
			for (j = 1; j < argc - 2; ++j)/**/
			{
				FILE* in = fopen(argv[j], "r");
				char t = '\n'; int flag = 0;
				while (t != '\0')
				{
					
					fscanf(in, "%c", &t);
					if (!(t > 65 && t < 97 || t == '-' || t == '_'))/* if not symbol t int a word*/
					{
						if (flag == 0) Number[j]++;
						flag = 1;
					}
				flag = 0;
				}
			}	
		} 
		else
		{
			wait();
			int k;
			for (k=0; k<argc-2; ++k){
				printf("%dwords --> %d\n", k, Number);
			}
		}
	}
	return 0;
	
}

