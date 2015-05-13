#include<stdio.h>
#include<string.h>
#include <unistd.h>

//где потоки?
int main(int argc, char** argv)
{

	if(argc!=3)
	{
		printf("Данные введены некорректно");
		return(1);
	}
	FILE* myfile;
	myfile=fopen(argv[1], "r+");
	int i;
	int numst;
	numst+=argv[2][0]-'0';
	int numch;
	char c;
	numch=0;
	while(!EOF)//EOF это вообщето константа, а не глобальная переменная.
	{
		fscanf(myfile, "%c", &c);
		//=getc(myfile); 
		numch++; 
	}
	int j;
	int numwords;
	int numofstrings;

	numwords=0;
	numofstrings=0;
	for(i=0;i<numst;i++)
	{
		j=fork();
		if(j==0)
		{
			int k;
			k=i*numch/numst;
			fseek(myfile, k*sizeof(char), SEEK_SET);
			int st;
			st=0;
			int l;
			
			while(k<(i+1)*numch/numst)
			{
				k+=1;
				char q;
				q=fgetc(myfile); 
				if((q==' ')&&(st==0))
				{
					st=1;
					numwords++;
				}
				else
				if((q!=' ')&&(st==0))
					st=0;
					
				if(q=='\n')
					numofstrings++;
			}
			break;
		}
		else
		if(i==numst-1)
		{	
			printf("Количество слов: %d\n", numwords);
			printf("Количество строк: %d", numofstrings);	
			//Ну тут,в общем, не сделан порядочным образом pipe, вот и не работает
		}	
	}	 
	
	
	
	return(0);
	
}
