#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if (argc != 3 || strlen(argv[1]) != strlen(argv[2])){
		printf("Wrong input\n");
		return 1;
	}
	char table[256];
	int i;
	for (i = 0; i < 256; ++i){
		table[i] = (char)i;
	}
	for (i = 0; i < strlen(argv[1]); ++i){
		table[(int)(argv[1][i])] = (char)(argv[2][i]);
	}
	
	int c = getchar();
	while (c != '\0' && c != '\n'){
		printf("%c", table[c]);
		c = getchar();
	}
	return 0;
}
