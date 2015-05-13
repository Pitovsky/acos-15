#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <math.h>

int main(int argc, char** argv){
 	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("can't open file\n");
		_exit(EXIT_FAILURE);
	}
	struct stat info;
	fstat(fd, &info); 
	printf("size %d\n", info.st_size); 
	char *file_addr =(char*)mmap(0, info.st_size,  PROT_READ, MAP_SHARED, fd, 0); 
	if(file_addr == NULL){
		perror("map failed");
		return -1;
	}
	unsigned int n; //number of pairs
	scanf("%d", &n);//нет, сразу -5 баллов за это.
	int* a = (int*)malloc((n+1)*sizeof(int));
	int* b = (int*)malloc((n+1)*sizeof(int));
	unsigned int i, j;
	int *count = (int*)calloc(256,sizeof(int));
	for(i = 0; i < n; i++){
		scanf("%d%d", &a[i], &b[i]);
		if(b[i] < a[i] || b[i] > info.st_size){
			perror("wrong pair");
			_exit(EXIT_FAILURE);	
		}
	}
	double enthropy = 0;
	unsigned char k;
	double p;
	for(i = 0; i < n; i++){
		for(j = a[i]; j < b[i]; j++){	
                	++count[*(file_addr + j)];
			printf("%d\n", count[*(file_addr + j)]);
        	}
		for(k = 0; k < 255; k++){
			p = count[k]/(b[i]-a[i]);
			printf("%lf\n", p);
			if(count[k] != 0)
				enthropy +=(-1)* p * log2(p);
				
		}
		printf("enthropy for %d  pair is: %lf\n", i,  enthropy);
		for(j = a[i]; j < b[i]; j++){
                	count[j] = 0;
		}
        	
	}
	return 0;
}
