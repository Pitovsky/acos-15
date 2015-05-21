#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
 

int main(int argc, char * argv[])
{
  char* FIFO_NAME = getenv("urls_src");
  FILE * f;
  char ch;
 // char* s;
  mkfifo(FIFO_NAME, 0600); 
  f = fopen(FIFO_NAME, "w");
  if (f == NULL) 
  { 
    printf("Не удалось открыть файл\n");
    return -1;
  }
  do
  {
    ch = getchar();
    fputc(ch, f);
    //s = gets(s);
   // fputc(ch, f);
    if (ch == '\n') fflush(f);//передает строку после enter
  } while  (ch != '0');
  fclose(f);
  unlink(FIFO_NAME);
  return 0;
}
