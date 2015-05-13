#include <stdio.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>

int main()
{
	char path[200];//маловато будет, есть константа для этого
	char pa;
	scanf("%c", &pa);
	int i;
	i=0;
	while(pa!='\n')
	{	
		path[i]=pa;
		scanf("%c", &pa);
		i++;
	}
	//scanf("%s", path);
	FILE* myfile;
	myfile=fopen(path, "r");
	while(1)
	{
		int start;
		int finish;
		scanf("%d %d", &start, &finish);
		fseek(myfile, start, SEEK_SET);//прямое нарушение условия: ноль баллов
		int i;
		char symb[256];
		int col[256];
		for(i=0;i<256;i++)
		{
			symb[i]=' ';
			col[i]=0;
		}
		int numofsymb;
		numofsymb=0;
		for(i=0;i<((finish-start)/sizeof(char));i++)
		{
			char c;
			fscanf(myfile, "%c",&c);
			int q;

			q=-1;
			int j;
			for(j=0;j<numofsymb;j++)
				if(symb[j]==c)
					q=j;
			if(q==-1)
			{
				symb[numofsymb]=c;
				col[numofsymb]++;
				numofsymb++;
			}
			else
			{
				col[q]++;
			}
		}
			
		for(i=0;i<numofsymb;i++)
		{
			double x;
			x= ((finish-start)/sizeof(char));
			x=col[i]/x;
			//x=x* log(x);
			printf("%c : %lf", symb[i], x);
		}
	}
	return(0);
}
	
	
					
