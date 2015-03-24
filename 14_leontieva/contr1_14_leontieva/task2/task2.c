#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){

if (argc <3){ perror("not enough arguments");
	return 0;
}

if (strlen(argv[1]) !=strlen(argv[2])){ perror("different length of args");
	return 0;
}

char input[20];
gets(input);
char* k=&input[0];
int flag=-1;
while (k!=NULL){
	k=strpbrk(input, argv[1]);
	int i;
	if (k!=NULL && &(input[flag]) < k){
		for(i=0; i <= strlen(argv[1]);i++)
			if (argv[1][i] == *k) break;
		strncpy(k, &argv[2][i],sizeof(char));
	flag=k;
	}
}
printf("%s\n", input);
return 0;
}
//flag не успел заработать, к сожалению
