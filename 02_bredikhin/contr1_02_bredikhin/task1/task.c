#include <stdio.h>

int I_WANT_TO_BELIEVE()
{
	int k;
	unsigned int test = 1;
	void* t = &test;
	unsigned short int* c = (unsigned short int*)t;
	if (*c == 1)
		k = 256 * 256;
	else k = 1;
	return k;
}

unsigned short int* convert(unsigned int* a)
{
	unsigned int b;
	int N = I_WANT_TO_BELIEVE();
	b = ((*a) % (N));
	void* b1 = &b;
	unsigned short int* c = (unsigned short int*)b1;
	return c;
}

int main()
{
	unsigned short int* b;
	unsigned int a;
	scanf("%d", &a);
	b = convert(&a);
	printf("%d", *b);
	return 0;
}
