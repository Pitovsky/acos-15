#include <stdio.h>

unsigned short int* convert(unsigned int* a)
{
	return (unsigned short int*)a; 
}

int main()
{
	unsigned int a;
	scanf("%d", &a);
	printf("%d", *convert(&a));
	return 0;
}
