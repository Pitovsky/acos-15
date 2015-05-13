#include<stdio.h>
#include<unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<stdlib.h>


double entropy(void* m_file, int begin, int end)
{
	char* beg_it = (char*)m_file + begin;
	char* end_it = (char*)m_file + end;
	int len = begin - end;
	int rs[256];
	double res = 0;
	int i;
	for (i = 0; i < 256; i++)
		rs[i] = 0;
	while (beg_it != end_it)
	{
		rs[*beg_it]++;//нужно приводить к беззнаковому типу
		beg_it++;
	}
	double freq;
	for (i = 0; i < 256; i++)
	{
		if (rs[i] != 0){
			freq = ((double)rs[i]) / len;
			if(freq!=0)
			res += ((-1) * freq * log2(freq));
		}
	}
	
	return res;
}


int main(int argc, char** argv){
	if (argc == 1){
		printf("Few arguments.\n");
		return 1;
	}
	int path = open(argv[1], O_RDONLY);
	struct stat fileinfo;
	if(fstat(path, &fileinfo)<0){
		printf("File doesn't exist.\n");
		return 1;
	}	
	void* m_file = mmap(0, fileinfo.st_size, PROT_READ, MAP_SHARED, path, 0);
	int begin, end;
	while(1){//про это на контрольной было сказано немыслимое число раз
		scanf("%d %d", &begin, &end);
		double ent = entropy(m_file, begin, end);
		printf("%lf\n", ent);
	}


	return 0;
}
