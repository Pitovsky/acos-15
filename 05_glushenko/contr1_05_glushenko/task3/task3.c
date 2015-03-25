#include <stdio.h>
#include <unistd.h>

int Count(char* file, size_t it){
	FILE* f = fopen(file, "W");
	int sum = 0;
	if(f != NULL){
		char ch = fgetc(f);
		while(ch != EOF && ch != 0){
			if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '-') || (ch == '_')){
				
			}else{
				if((fgetc(f) >= 'a' && fgetc(f) <= 'z') || (fgetc(f) >= 'A' && fgetc(f) <= 'Z') || (fgetc(f) == '-') || (fgetc(f) == '_') || (fgetc(f) == EOF))
					sum++;
			}
			ch = fgetc(f);
		}
	}
	printf("%s:\t%d\n", file, sum);
	return 0;
}

int main(int argc, char** argv){

	if(argc <  3){
		fprintf(stderr, "Error, too less args");
		return 1;
	}
	int n;
	sscanf(argv[argc-1], "%d", &n);
	int i = 0;
	pid_t fk[n];
	for(i = 0; i < n; i++){
		fk[i] = -1;
	}
	for(i = 0; i < n; i++){
		fk[i] = fork();
	}
	size_t it = -1;
	for(i = 1; i < n; i++)
		if(fk[i] == 0)
			it = i;
	for(i = it; i < argc - 2; i++){
		return Count(argv[it], it);
	}
	/*for(i = 0; i < argc - 2; i++){
		sum[i] = Count(argv[it]);
		it++;
	} */
	return 0;
}
