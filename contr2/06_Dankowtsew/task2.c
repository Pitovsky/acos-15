#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>

int count[256];

int fcount(int start, int end, char* ptr)
{
	int i;
	for (i = start; i < end; ++i)
	{
		//printf("%c\n", ptr[i]);
		count[(int)ptr[i]]++;//неверно, приводить нужно к беззнаковому типу. если попадётся символ с кодом > 127 (отрицательный) будет segfault
	}
}

int main(int argc, char** argv)
{
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
	}
	struct stat info;
	if (fstat(fd, &info) == -1)
    {
        perror("Can't get file size.\n");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", info.st_size);

	void* ptr = mmap(NULL, info.st_size,
                    PROT_READ,
                    MAP_SHARED, fd, 0);
	if (ptr == NULL)
	{
		perror("mmap");
	}
	while(1)
	{
		int a, b;
		int code = scanf("%d%d", &a, &b);
		if (code <= 0)
		{
			break;
		}
		if (a == b)
			continue;//вывести ноль было бы правельнее

		memset(count, 0, 256);
		//printf("!!!!!!!!!!%d %d\n", a,b);
		fcount(a, b, (char*)ptr);
		int i;
		//printf("!!!!!!!!!!%d %d\n", a,b);
		double res = 0;
		for (i = 0; i <256; ++i)
		{
			if (count[i] != 0)
			{
				printf("%c %d\n", (char)i, count[i]);
				double freq = ((double)count[i]) / (b - a);
				res += freq * log(freq) / log(2);
				printf("freq = %lf\n", freq);
			}
		}
		printf("Result = %lf\n", (-1)*res);
	}
	munmap(ptr, info.st_size);
	return 0;
}