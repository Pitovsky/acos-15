#include <stdio.h>

unsigned short int* convert(unsigned int* A){
 return (unsigned short*)A;
}
int main(){
unsigned int A=255*256;
printf("%d\n", *convert(&A));
return 0;}
