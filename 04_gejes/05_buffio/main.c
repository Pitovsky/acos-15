#include <stdio.h>
#include <stdlib.h>

#include "myfio.h"

int main(int argc, char** argv)
{
    myFile* fout = myfopen("testfile.txt", "wt");
    char testtext[256];
    scanf("%s", testtext);
    myfwrite(fout, testtext, strlen(testtext));
    myfclose(fout);
    printf("Wrote and closed.\n");
    memset(testtext, 0, 256);
    myFile* fin = myfopen("testfile.txt", "r");
    myfread(fin, testtext, 256);
    myfclose(fin);
    printf("Readed:\n%s\n", testtext);

    return 0;
}
