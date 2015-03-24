#include <stdio.h>
#include <string.h>
#include <unistd.h>

void process_child(int read_pipe, int id)
{}

int main(int argc, char** argv){
int number;

   sscanf(argv[argc - 1], "%d", &number);

if (argc <3){ perror("not enough arguments");
	return 0;
}

int task[2];
   pipe(task);
int i;
for (i=1; i<argc-1; ++i){
      write(task[1], &argv[i], sizeof(char**));
}
int child;
   
for (i = 0; i < number; ++i){
	child = fork();
	if (child == 0){
          process_child(task[0], i);
	}
}
	return 0;
}

