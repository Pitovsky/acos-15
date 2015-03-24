#include "unistd.h"
#include "stdio.h"
#include "string.h"

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: usage %s path path\n", argv[0]);
        return 1;
    }

   
    char outStrFirst[255][255];
    char outStrSecond[255][255];
    int outSizeFirst = 0;
    int outSizeSecond = 0;

    pid_t second;
    pid_t first = fork();
    if (first != 0)
    { 
        wait(first);
        int i = 0;
        while (fgets(outStrFirst[i], 255, stdout) != NULL)
            ++i;
        outSizeFirst = i;
        second = fork();
    }

    if (first == 0)
    {
        int code = execl(argv[1], argv[1], NULL);
        if (code != 0)
        {
            fprintf(stderr, "Error in first program: code %d\n", code);
        }
        return code;
    }
    else if (second == 0)
    {
        int code = execl(argv[2], argv[2], NULL);
        if (code != 0)
        {
            fprintf(stderr, "Error in second program: code %d\n", code);
        }
        return code;
    }

    else
    {    
        wait(second);
        int i = 0;
        while (fgets(outStrSecond[i], 255, stdout) != NULL)
            ++i;
        outSizeSecond = i;
        for (i = 0; i < max(outSizeFirst, outSizeSecond); ++i)
            printf("%s\t%s\n", outStrFirst[i], outStrSecond[i]);
    }

    return 0;
}
