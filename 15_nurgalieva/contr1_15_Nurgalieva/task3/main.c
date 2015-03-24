#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int countW(FILE* f)
{
 int c;
 int j = 0;
 int count1 = 0; 

 while ((c = getc(f)) != EOF )
 {
  while ((c != ' ') && (c != EOF))
   {
   c = getc(f);
   }
  ++count1;
 }
return count1; 
}

int main(int argc, char* argv[])
{
  int i = 1;
  int j = argc - 3; /*кол файлов*/
  int l = 0;

  int count = atoi(argv[argc -1]); /*кол процессов*/
  int mas[count];
           
	   for ( l = 0 ; l < count; ++l)
	   {
	   mas[l] = fork();
           if ( mas[l] == 0)
           {
	    for (i = 1; i< j; ++i)
             { 
	     FILE* f = fopen(argv[i], "r");
	     printf ("%s  %d\n", argv[i], countW(f));
	     }
	    }
	   else 
           {
	     wait();
	     printf("The end\n");
           }
       	}
	    
 return 0;
}
