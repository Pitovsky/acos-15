#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr, "Error");
		return 1;
	}
	if(strlen(argv[1]) != strlen(argv[2])){
		fprintf(stderr, "Wrong Input");
		return 2;
	}
	char ch = getchar();
	while(ch != EOF && ch != 0){
		int i;
		char ex = ch;
		for(i = 0; i < strlen(argv[1]); i++){
			if(ch == argv[1][i]){
				ex = argv[2][i];
			}
		}
		ch = getchar();
		putchar(ex);
		
	}
	return 0;
}
