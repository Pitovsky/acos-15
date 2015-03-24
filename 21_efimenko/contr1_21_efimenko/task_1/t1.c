#include<stdio.h>

/*int endian(){
	unsigned short x = 1;
	return (*((unsigned char*) &x)== 0 ? 1 /*big*/// : 0);


unsigned short int* convert(unsigned int* A)
{
	(*A) = (*A)%(256*256);
	return ((unsigned short int*)(*A));

}

int main(){
	unsigned int *A;
	
	scanf("%i", (*A));
	unsigned short int *B = convert(A);
	printf("%i", (*B));
	

	return 0;
}
