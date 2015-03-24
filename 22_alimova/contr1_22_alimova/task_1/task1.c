#include <stdio.h>


unsigned short int* convert(unsigned int* a)
{
	unsigned short int* s = (unsigned short int*)a;
	*s = *a % (256 * 256);
	return s;
}

int main()
{
	unsigned int a;
	unsigned short int* x;
	scanf("%d", &a);
	x = convert(&a);
	printf("%d", *x);
	printf("\n");
}