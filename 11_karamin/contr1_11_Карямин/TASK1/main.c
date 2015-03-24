#include <stdio.h>
unsigned short int* convert(unsigned int* a)
{
	unsigned short int* p = (unsigned short int*)a;
	*p = *a % (256 * 256);
	return p;
}

int main()
{
	unsigned int a;
	unsigned short int* b;
	scanf("%d", &a);
	//a = 23443;
	b = convert(&a);
	printf("%d", *b);
	
	return 0;
}