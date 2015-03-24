#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void child_task(int begin, int step, int argc, char** argv)
{
        printf("in child_task %d\n",begin);
        int i,count;
        char stream, stream_pr;
       
	FILE* fp;
	for (i=begin; i<argc-1;i+=step)
	{
		printf("in for\n");
		fp = open(argv[i],"r");
		printf("goodbye open\n");
		if (fp == NULL) {printf("NULL\n");}
                count = 0;
		printf("hello getc\n");
                //int a = fread(&stream_pr,1,1,fp);
		fscanf(fp,"%c",&stream_pr);	
		//printf("%d\n",a);
		printf("goodbye getc\n");
                while (1)
                {
                        printf("not feof\n");
			stream = getc(fp);
                        while (!feof(fp))
                        {
                                if (((stream == " ") && ((stream_pr != " ") || (stream_pr != "\n"))) || ((stream == "\n") && ((stream_pr != " ") || (stream_pr != "\n"))))
                                {
                                        ++count;
                                }
                                stream_pr = stream;
                                stream = getc(fp);
                        }
			if (stream == EOF)
			{	
				break;
			}
                }
                printf("%s\t%d\n",argv[i],count);
        }
}

int main(int argc, char** argv)
{
        int i;
        int number;
        sscanf(argv[argc-1],"%d",&number);

        int pid;
        for (i=1; i <= number;i++)
        {
                pid = fork();
                if (pid == 0) //child
                {
                        printf("in child %d\n",i);
                        child_task(i,number, argc, argv);
                }
        }
        return 0;
}
