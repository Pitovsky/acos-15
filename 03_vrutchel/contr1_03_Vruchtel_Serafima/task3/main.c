#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void process_child(char *name, int id)//один ребёнок считает число слов в одном из файлов
{
   FILE *file = fopen(name, "r");
   if (file != NULL)
   {
      int sum_words = 0;
      char *ch;
      while (! feof(file))
      {
        ch = fgetc(ch);
        if(((ch >= 48) and (ch <= 57)) or ((ch >= 65) and (ch <= 90)) or ((ch >= 95) and (ch <= 122))) {
        } else sum_words++;
      }
      printf("%s\t %d", name, sum_words);
   }
   else
   {
      printf("Can't open the file\n");
      return;
   }
}

int main(int argc, char **argv)
{
    if(argc < 3){
        printf("Error\n");
        return -1;
    }
    int i;
    for(i = 0; i < argv[argc]; i++)
    {
        int retval = fork();
    }
    if(retval == 0) {//ребёнок

    }

    return 0;
}
