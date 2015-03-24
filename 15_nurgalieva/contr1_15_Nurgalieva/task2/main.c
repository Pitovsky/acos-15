#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char symSwap(char c, char* str1, char* str2)
{
 int i;
 int len = strlen(str1);
  
 for ( i = 0; i < len; ++i)
 {
  if (c == str1[i]) 
  { 
  return str2[i]; 
  }
 }
 return c;
}   

int main(int argc, char* argv[])
{
	char c;
	int len = strlen(argv[1]);
	char* str1 = (char*) malloc (len*sizeof(char));
	char* str2 = (char*) malloc (len*sizeof(char));

	str1 = argv[1];
	str2 = argv[2];
	
	while (( c = getchar()) != EOF)
	{
	printf("%c", symSwap(c, str1, str2));
	}
   return 0;
}	  
