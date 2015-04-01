#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char** argv){
	char* EnvVar = getenv("URLS_SRC");

	FILE* Pipe = open(EnvVar);
	if(Pipe == NULL){
		mkfifo(EnvVar);
		Pipe = open(EnvVar);
	}
	
	char URL[256];
	while(read(Pipe, URL, 256) != 0){
		printf("Current: %s\n", URL);
		int URLPipe[2];
		pipe(URLPipe);
		pid_t child = fork();
		if(child != 0){
			char HREF[256];
			close(URLPipe[1]);
			Printf("code\n");
			while(read(URLPipe[0], HREF, 256) != 0){
				int i = 4;
				while(i < 256 && HREF[i] != 0){
					if(HREF[i - 3] == 'h' &&\
					   HREF[i - 2] == 'r' &&\
					   HREF[i - 1] == 'e' &&\
					   HREF[i] == 'f'){
						printf("href: ");
						while(HREF[i] != '"')
							i++;
						i++;
						while(HREF[i] != '"'){
							printf("%c", HREF[i]);
							i++;		
						}
						printf("\n");
					}
					i++;
				}
			}
			close(URLPipe[0]);
		}
		else{
			close(URLPipe[0]);
			printf("curling...\n");
			dup2(URLPipe[1], 1);
			close(URLPipe[1]);
		}	
	}
	fclose(Pipe);
	return 0;
}

