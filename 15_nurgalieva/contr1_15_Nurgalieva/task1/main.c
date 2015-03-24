#include <stdio.h>

unsigned short int* convert(unsigned int* a)
{
  unsigned short int* b = (unsigned short int*) a;
  *b = ((*a) % (256*256)); 
  return b;
}

int main()
{
  unsigned int  a;
  scanf("%d", &a);
  unsigned short int* c = convert(&a);
  printf("%d", *c);
  return 0;
}
