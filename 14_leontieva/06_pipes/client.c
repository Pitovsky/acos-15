#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void curl(){

}
int main ()
{
  char* FIFO_NAME = getenv("urls_src");
  FILE * f;
  char* url;
  f = fopen(FIFO_NAME, "r");
  do{ 
    fscanf(f,"%s", url );
    if (strcmp(url, "0") != 0){ 
      printf("%s\n",url); 
      curl();
    }
  } while (strcmp(url, "0") != 0);
  fclose(f);
 // unlink(FIFO_NAME);
  return 0;
}
