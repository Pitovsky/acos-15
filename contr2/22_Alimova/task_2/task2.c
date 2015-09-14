#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<fcntl.h>
#include<sys/mman.h>

double letter_entropy (void* begin, int start, int end);

int main(int argc, char* argv[])
{
	int ch=1;
	int start, end;
	int f=open(argv[1], O_RDONLY);
	struct stat fileinfo;
	fstat(f, &fileinfo);
	void* begin = mmap(0, fileinfo.st_size, PROT_READ, MAP_SHARED, f, 0);
	while (ch!=0)//плохой способ проверки.
	{
		ch = scanf("%d%d", &start, &end);
		printf("%lf\n", letter_entropy(begin, start, end));
	}
	return 0;
}

double letter_entropy (void* begin, int start, int end)
{
	int len = end-start;
	int i;
	int a[256];
	double sum = 0, p;
	char* p1 = (char*)begin + start;
	char* p2 = (char*)begin + end;
	for(i=0; i<256; ++i)
		a[i]=0;
	while (p1!=p2)
	{
		a[*p1]++;//к беззнаковому типу нужно приводить.
		p1++;
	}
	for(i=0; i<256; ++i)
	{
		p=((double)a[i])/len;
		if(p!=0)
			sum+=((-1)*p*log2(p));
	}
	return sum;
}
		



