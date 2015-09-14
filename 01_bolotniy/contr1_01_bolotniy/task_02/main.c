#include<stdio.h>
#include<string.h>

int main(int argc, char **argv){
	int symb;
	int i;
	FILE *f = fopen("input.txt","r");
	symb = fgetc(f);
	while (symb != EOF){
		for(i = 0; i < strlen(argv[1]); i++){
			if (symb == argv[1][i]){
				symb = argv[2][i];
				break;
			}
		}
	printf("%c",symb);
	symb = fgetc(f);
	}
	return 0;
}
