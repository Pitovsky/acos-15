#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<math.h>

int main(int argc,char** argv){
	int fd = open(argv[1],O_RDONLY);

	if (fd == -1){
		perror("can`t open");
		_exit(1);
	}

	struct stat st;
	stat(argv[1],&st);

	void *file_addr = mmap(NULL,
				st.st_size,
				PROT_READ,
				MAP_SHARED,
				fd,
				0);

	if (file_addr == NULL){
		perror("map failed");
		return 1;
	}
    //оформление ужасно
int a;
int b;
scanf("%d%d",&a,&b);
int num = b-a+1;
char *mas = (char *)file_addr+a;
int tmp;
int k;
int i;
double lg;
double lg2 = log(2.0);
double entropy = 0;
double nk;
for (tmp = 0;tmp<256;tmp++){//не оптимальное
for (i=0;i<num;i++){
if (*(mas+i) == tmp) {k++;}
}
nk = k/num;
if (nk != 0) {
lg = log(nk);
entropy+=(nk*lg/lg2);
}
k=0;
}
printf("%lf",entropy);
return 0;
}
