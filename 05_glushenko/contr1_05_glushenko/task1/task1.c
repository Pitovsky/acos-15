#include <stdio.h>

unsigned short int* convert(unsigned int* number){
	unsigned short int* r = number;
	if(*r == *number)
		return r;
	else
		return r+1;
}

int main(){
	unsigned int a;
	scanf("%d", &a);
	printf("%d", *convert(&a));
	return 0;
}
