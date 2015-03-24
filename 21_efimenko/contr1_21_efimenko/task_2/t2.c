#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){
	if (argc<3){
		perror("so little argument");
		return 1;
	}
	else{
		if (strlen(argv[1]) != strlen(argv[2])){
			perror("different lengths of arguments");
			return 1;
		}
		char* s = (char*)malloc(255*sizeof(char));
		scanf("%s", s);
		int i, k;
		for(i = 0; i< strlen(s); ++i){
			k = 0;
			while((k!=strlen(argv[1])) && (argv[1][k] != s[i])) ++k;
			if (k!=strlen(argv[1]))
				s[i] = argv[2][k];
		}
		printf("%s\n", s);
		return 0;
	}
}
