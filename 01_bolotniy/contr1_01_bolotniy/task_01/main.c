#include<stdio.h>

unsigned short int * convert(unsigned int * a){
	return a;
}

int main(){
	int a;
	scanf("%d",&a);
	unsigned short int* b;
	b = converter(&a);
	printf("%d",*b);
}
