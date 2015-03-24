#include <stdio.h>
#include <string.h>

char process_char(char x, char* src, char* dst)
{
   for (int i = 0; i < strlen(src); ++i)
   {
      if (src[i] == x)
         return dst[i];
   }
   return x;
}

int main(int argc, char** argv)
{
   if (argc != 3)
   {
      //fprintf("Wrong input.");
      return 1;
   }

   char *s1 = argv[1], *s2 = argv[2];

   if (strlen(s1) != strlen(s2))
   {
      return 1;
   }

   char c;
   while (! feof(stdin))
   {
      c = getc(stdin);
      if (! feof(stdin))
         printf("%c", process_char(c, s1, s2));
   }
   
   return 0;
}
