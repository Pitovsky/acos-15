#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int process_file(char *path)
{
    int count = 0, last = 0;
    char ch;
    FILE *current_file = fopen(path, "r");

    if (current_file == NULL)
    {
        fprintf(stderr, "Can't open file: %s\n", path);
        return 0;
    }

    while(1)
    {
        ch=fgetc(current_file);

        if (!((ch >= '0' && ch <='9') ||
            (ch >= 'a' && ch <='z') ||
            (ch >= 'A' && ch <='Z') ||
            (ch =='_')))
        {
            if (last != 0)
            {
                count++;
            }

            last = 0;
        }
        else
        {
            last++;
        }

        if (ch == EOF)
        {
            break;
        }
    }

    fclose(current_file);
    return count;
}

int main(int argc, char **argv)
{
   if (argc < 2)
   {
      goto wrong_input;
   }

   int number;

   sscanf(argv[argc - 1], "%d", &number);

   int child;
   
   for (int i = 0; i < number; ++i)
   {
       child = fork();
       if (child == 0)
       {
	  int id = i+1;
          for (int k = 0; id + number*k < argc -1; ++k)
          {
              printf("%s %d\n", argv[id + number*k], process_file(argv[id + number * k]));
          }
          return 0;
       }
   }
   void *status = NULL;
   for (int i = 0; i < number; ++i)
      wait(status);
   return 0;

wrong_input:
   fprintf(stderr, "Wring input\n");
   return 1;

}

