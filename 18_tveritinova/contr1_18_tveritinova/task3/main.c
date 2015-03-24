#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void child_task(int input, int id)
{
	printf("in child_task %d\n",id);
	char** str;
	int i,count;
	char stream, stream_pr;
	read(input, str, sizeof(char**));
	FILE* fp = open(*str, "r");
	
	if (feof(input))
	{
		printf("eof\n");
	}
	while (!feof(input))
	{
		printf("str not eof\n");
		i=0;
		count = 0;
		stream_pr = getc(fp);
		if (stream_pr != EOF)
		{
			stream = getc(fp);
			while (stream != EOF)
			{
				if (((stream == " ") && ((stream_pr != " ") || (stream_pr != "\n"))) || ((stream == "\n") && ((stream_pr != " ") || (stream_pr != "\n"))))
				{
					++count;
				}
				stream_pr = stream;
				stream = getc(fp);
			}
		}
		printf("%s\t%d\n",*str,count);
		read(input, str, sizeof(char**));
	}
}	  

int main(int argc, char** argv)
{
	int task_pipes[2];
	pipe(task_pipes);
	int i;
	int number;
	sscanf(argv[argc-1],"%d",&number);
	for (i=1;i<argc-2;i++)
	{
		write(task_pipes[1],&argv[i],sizeof(char**));
	}
	
	int pid;
	for (i=0; i < number;i++)
	{
		pid = fork();
		if (pid == 0) //child
		{
			printf("in child %d\n",i);
			child_task(task_pipes[0], i);
		}
	}
	return 0;
}
