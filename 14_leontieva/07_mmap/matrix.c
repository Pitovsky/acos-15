#include <fcntl.h> 
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

void getinfo (int m, int n){
	printf("number of columns = %d\n",  m);
	printf("number of raws = %d\n" , n);
}
void getxy(char* pt, int m){
	int x,y;
	printf("enter coordinate\n");
	scanf("%d%d", &x,&y);
	int c = m*2*(y-1)+ 2*(x-1) ;
	printf("%c\n", *(pt+c));
}
void setxy(char* pt, int m){
	int x,y, num; char cnum[256];
	printf("enter a coordinate\n");
	scanf("%d%d", &x, &y);
	printf("enter a number\n");
	scanf("%d", &num);
	strncpy((pt+m*2*(y-1)+ 2*(x-1)) ,itoa(num,10), sizeof(itoa(num,10)));
}

int main(int argc, char** argv){
if (argc < 3){ perror("too few args"); exit(1);}

int m = atoi(argv[1]);//number of columns
int n = atoi(argv[2]);//number of raws

//printf("%d %d", m,n);
int fd;
struct stat statbuf;
char*pt;

if( (fd = open("./mmatrix", O_RDWR | O_TRUNC | O_CREAT, 0666) ) < 0){
	perror("can't create file for matrix");
	exit(1);
}
printf("%d\n", fd);
//if (lseek(fd, statbuf->st_size -1, SEEK_SET) == -1){//указатель смещается в конец файла
//	perror("lseek error");
//}

int i, j;
for (i = 0; i< n; i++)
	for(j = 0;j < m; j++)
		if (write(fd, "0 ", 2) != 2){
			perror("can't write to file");
			exit(1);
		}
//printf("%d\n", fd);
if (fstat(fd, &statbuf) < 0){ 
	perror("can't get stat"); 
	exit(1);
}

//printf("%d\n", statbuf.st_size);
pt = (char*)mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//printf("%d\n",  pt);
if (pt == MAP_FAILED){
	perror("mmap failed");
	exit(1);
}

char command[256];
do{
	printf("Do you want to give a command?(yes/no)\n");
	char* answ;
	scanf("%s", answ);
	if (strcmp(answ, "no") == 0) break;
	else if (strcmp(answ, "yes") == 0) {
		printf("What should i do?\n");
		scanf("%s", &command); //get command
		if (strcmp("getinfo", command) == 0) getinfo(m,n);
		if (strcmp("getxy", command) == 0) getxy(pt, m);
		if (strcmp("setxy", command) == 0) setxy(pt, m);
	}
	else perror("i don't understand");
} while (1);
close(fd); 
return 0;
}
