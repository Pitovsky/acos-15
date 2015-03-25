#include <stdio.h>
//ilnurkh: решения то нет
unsigned short int * convert(unsigned int* x)
{
   return (unsigned short int *) x;
}
int main()
{
   unsigned int a = 0xFFFF0000, b = 0x0000FFFF;
   printf("%hu  %hu\n", *(convert(&a)), *(convert(&b)));
   return  0;
}
