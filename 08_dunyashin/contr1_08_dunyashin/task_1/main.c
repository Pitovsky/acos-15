#include <stdio.h>
#include <stdlib.h>

unsigned short int* convert(unsigned int* a){
	return (unsigned short int*)a;
}

int main(){
	int a;
	scanf("%d", &a);
	printf("%d", *convert(&a));
	return 0;
}
