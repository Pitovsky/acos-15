#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void child_proc(int read_stream){
	while (1)
	{
		char* s;
		int words = 0;
		char symb;
		short flag;
		int task = read(read_stream, &s, sizeof(s));
		if (task == EOF) return;
		
		FILE* fs = fopen(s, "r");
		if (fs != NULL)
		{
			flag = 0;
			words = 0;
			while(!feof(fs)){
				symb = fgetc(fs);
			//поиск слов	
				printf("%c %i %i\n", symb, words, flag);
				if( ((symb >= 'a')&&(symb <= 'z')) || ((symb >= 'A')&&(symb <= 'Z')) || 
				((symb >= '0')&&(symb <= '9'))||
				(symb = '-') || (symb = '_'))
				
				{
					if (flag == 0) {flag =1;}
				}
				else {
					if (flag == 1){
						flag = 0;
						++words;
					}
				}
				
			}
			//printf("%s\t%i", s, words);
			
		}
		else{
			perror("Problem with open");
			return;
		}
	}
}

int main(int argc, char** argv){
	if (argc<3){
		perror("so few argument");
		return 0;
	}
	int num;
	sscanf(argv[argc-1], "%d", &num);
	int task_pipe[2];
	pipe(task_pipe);
	int i;
	for(i=1; i<argc-1; ++i)
		write(task_pipe[1], &argv[i], sizeof(char**));
	int child;
	for( i = 0; i < num; ++i)
	{
		child = fork();
		if (child == 0){
			 child_proc(task_pipe[0]);
		}
	}
	return 0;

}
