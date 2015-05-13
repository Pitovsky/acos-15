#include<stdio.h>
#include<string.h>
#include <unistd.h>

//и где потоки
int main(int argc, char** argv)
{
	FILE* myfile;
	myfile=fopen(argv[1], "r");
	int numstring, numchars, numwords;
	numstring = argv[2][0]-'0';
	char c;
	numchars=0;
	while(!EOF)
	{
		c=getc(myfile);
		numchars++;
	}
	int i, j;
	numwords=0;
	for(i=0;i<numstring;i++)
	{
		j=fork();
		if(j==0)
		{
			int k, s;
			k=i*numchars/numstring;
			fseek(myfile, k, SEEK_SET);
			int st;
			s = 0;
			while((c=fgetc(myfile))&& (c!=SEEK_SET+k))
			{
				char q;
				q=getc(myfile);
				if((q==' ')&&(s==0))
				{
					s=1;
					numwords++;
				}
				else	
				if((q!=' ')&&(s==0))
					s=0;
				if(q=='\n')
				numofstring++;
			}
		}	
	}	
	printf("%d %d\n", numstring, numwords);
	return(0);	
}
