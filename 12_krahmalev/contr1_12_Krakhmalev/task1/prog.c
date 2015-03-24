#include<stdio.h>
#include<stdlib.h>

unsigned short int* convert(unsigned int* s){
	unsigned short int* q;
	q=&s;
	return(*q);
}

int main(){
	unsigned int l=2;
	printf("%d", *convert(&l));
	return(0);
}
