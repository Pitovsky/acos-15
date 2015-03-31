#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* src = getenv("URLS_SRC");
    printf("finded pipe: %s\n", src);

    mkfifo(src, S_IFIFO | 0666);
    FILE* srcPipe = open(src, O_WRONLY);

    char newurl[255];
    while (strcmp(newurl, "exit") != 0)
    {
        scanf("%s", newurl);
        write(srcPipe, newurl, 255);
        //fprintf(srcPipe, "%s\n", newurl);
    }

    fclose(srcPipe);
    return 0;
}
