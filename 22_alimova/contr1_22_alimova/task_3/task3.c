#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void processchild(int read_pipe, int id)
{
   while (1)
   {
      int i;  
      char *str;
      int code = read(read_pipe, &str, sizeof(str));

      if (code == EOF)
      {
         return;
      }

      printf("%s \t ", str);
      FILE *file = fopen(str, "r");
      if (file != NULL)
      {
         int words = 0;
         char ch;
         while (! feof(file))
         {
          for(i=0;i<strlen(str);i++)
          {
           
            if(words>1)

            {
              ch = fgetc(file);

              if (str[i] == ' ' && str[i-1]!=' ' && str[i+1]!=' ')
                words ++;
            } 
             else {
               ch = fgetc(file);
              if (str[i] ==  ' ')
                words ++;
             }

         }
         printf("n_words = %d\n", words);
       }
      }
      else
      {
         perror("Nelzia otkrit\n");
         return;
      }
   }
}

int main(int argc, char **argv)
{
   if (argc < 2)
   {
      goto wrong_input;
   }

   int num;

   sscanf(argv[argc - 1], "%d", &num);

   int task[2];
   pipe(task);
   int i;
   for (i=1; i<argc-1; ++i)
   {
      write(task[1], &argv[i], sizeof(char**));
   }

   int child;
   int j;
   for (j = 0; j < num; ++j)
   {
       child = fork();
       if (child == 0)
       {
          processchild(task[0], j);
       }
   }
   
   return 0;

wrong_input:
   fprintf(stderr, "Nepravilnii vvod\n");
   return 1;

}


