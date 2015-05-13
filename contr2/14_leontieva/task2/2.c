#include <fcntl.h> 
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char** argv){

if (argc < 2){
	perror("too few arguments");
	return -1;
}

int fd;
char*pt;

fd = open(argv[1], 00400);
if (fd == -1) {
	perror("can't open file");
	return -1;
}

unsigned begin, end;
printf("enter a diap. in number of bytes\n");//почерму один раз?
scanf ("%d%d", &begin, &end);//if corect

struct stat f;
if (stat(argv[1], &f) == -1) {
	perror("stat");
	return 0;
}
if (f.st_size < end+1){
	printf("incorrect diap.\n");
	return 0;
}
pt = (char*)mmap(0, end - begin, PROT_READ, MAP_SHARED, fd, begin);
if (pt == MAP_FAILED){
	perror("mmap failed");
	exit(1);
}
//printf("%c",*pt); 
int i, j,k = 0, flag; double ch, entro = 0;
char symbols[256];int len = 1;
for (j = 0; j < end - begin; j++){//по всем символам -выбираем тот,для которого считаем частоту
	char c = *(pt+j);
	//проверка на ужевстрчаемость--//
	for (k = 0; k < len; k++){//мене сложность этого алгоритма не устраивает
		if (c == symbols[k]) {
			flag = 1;
			break;
		}
	}
	if (flag == 1){
		flag = 0;
		continue;
	}
	symbols[len] = c;	len++;
	//---//
	int count_c = 0;
	for (i = j; i < end - begin; i++){//частота встречания символа
		if (c == *(pt+i)) count_c++;
	}
//	printf("%d\n", count_c);
	ch = (end - begin)/count_c;
	entro += ch * (log(ch)/log(2));
}
printf("%e\n", entro);
close(fd);
return 0;
}


