#include "stdio.h"
#include "string.h"
#include "unistd.h"

int strToInt(const char* str, int *ans)
{
    int i = strlen(str) - 1;
    int eps = 1;
    *ans = 0;
    for (i = strlen(str) - 1; i >= 0; --i)
    {
        if (str[i] < '0' || str[i] > '9')
            return -1;
        *ans = *ans + eps * (str[i] - '0');
        eps = eps * 10;
    }
    return 0;
}


int main(int argc, char** argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Error: nothing to count. Use with -p int\n");
        return 1;
    }
    
    int i = 0;
    int cproc = 0;
    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-p") == 0)
        {
            if (strToInt(argv[i + 1], &cproc) != 0)
            {
                fprintf(stderr, "Error: usage %s args... -p int args...\n", argv[0]);
                return 2;
            }
            break;
        }
    for (i = i; i < argc; ++i)
        argv[i] = argv[i + 2];
    argc = argc - 2;
    if (cproc > argc)
        cproc = argc;

    pid_t fk[cproc];
    for (i = 0; i < cproc; ++i)
        fk[i] = -1;
    int bigPapa = getpid();
    for (i = 0; i < cproc; ++i)
        if (getpid() == bigPapa)
        {    
            fk[i] = fork();
        }

    int nmb = -1;
    for (i = 0; i < cproc; ++i)
        if (fk[i] == 0)
            nmb = i;
            
    if (nmb != -1)
    {
        for (i = nmb + 1; i < argc; i = i + cproc)
        {
            int count = 0;
            FILE* f = fopen(argv[i], "r");
            char word[1023];
            while (fscanf(f, "%s", word) == 1)
            {
                //printf(" -- %d say from %s: word %d: %s\n", nmb, argv[i], count, word);
                count++;
            }
            fclose(f);
            printf("%s\t%d\n", argv[i], count);
        }
        return 0;
    }
    else if (getpid() == bigPapa)
        for (i = 0; i < cproc; ++i)
            wait();

    return 0;
}



