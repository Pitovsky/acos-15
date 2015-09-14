#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	struct stat statbuf;
	int fdin = open(argv[1], O_RDONLY);
	fstat(fdin, &statbuf);
	char* first;
	int i, left, right;
	float entr;
	int* density = (int*)malloc(256 * sizeof (int));
	void* begin = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
	while(1){
		entr = 0;
		for (i = 0; i < 256; ++i){
			density[i] = 0;
		}
		int err = scanf("%d%d", &left, &right);
		if (err != 2){
		break;}
		for (i = left; i < right && i < statbuf.st_size; ++i){
			density[*((char*)begin + i)]++;//надо использовать беззнаковый указатель
		}
		if (right > left){
			for (i = 0; i < 256; ++i){
				if (density[i] != 0){
					entr -= ((float)density[i])/(right - left) * log(((float)density[i])/(right - left)) / log(2); 
					printf("%c %d\n", i, density[i]);
				}
			}
		}
		printf("%f\n", entr);
	}
	return 0;
}
