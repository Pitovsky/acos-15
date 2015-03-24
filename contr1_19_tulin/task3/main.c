#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void printUsage()
{
    printf("WASTED!!!11!11\nThisProg [filenames] -p 'process qty'\n");
    exit(1);
}

typedef struct
{
    int n;
    char **filenames;
} ChildArgument;

void childProcess(int id, ChildArgument *chieldArgument)
{
    for (int i = 0; i < chieldArgument->n; i++)
    {
        int fd = open(chieldArgument->filenames[i], O_RDONLY);
        if (fd < 0)
        {
            perror("Can't read a file");
            exit(0);
        }
        
        char c;
        int qty = 0;
        while (read(fd, &c, 1) != 0)
        {
            if (!((c >= 'a' && c <= 'z') || (c >= '0' && c<= '9') || (c >= 'A' && c <= 'Z') || c == '-' || c== '_'))
            {
                qty++;
            }
        }
        printf("%s\t%d\n", chieldArgument->filenames[i], qty);
        close(fd);
    }
    exit(0);
}

int main(int argc, char **argv)
{
    int processQty = 0;
    char **filenames = (char **)calloc((argc - 3), sizeof(char *));
    
    for (int i = 1, j = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            i++;
            processQty = atoi(argv[i]);
        }
        else
        {
            filenames[j] = argv[i];
            j++;
        }
    }
    
    if (!processQty)
        printUsage();
    printf("ProcessQty: %d\n", processQty);
    
    ChildArgument *chieldArguments = (ChildArgument *)calloc(processQty, sizeof(ChildArgument));
    
    for (int i = 0; i < argc - 3; i++)
    {
        if (!(i / processQty))
        {
            (chieldArguments[i % processQty]).filenames = (char **)calloc(1, sizeof(char *));
        }
        else
        {
            int n = (chieldArguments[i % processQty]).n;
            (chieldArguments[i % processQty]).filenames = (char **)realloc((chieldArguments[i % processQty]).filenames, (n+1) * sizeof(char *));
        }
        int n = (chieldArguments[i % processQty]).n;
        (chieldArguments[i % processQty]).filenames[n] = filenames[i];
        (chieldArguments[i % processQty]).n++;
    }

    for(int i = 0; i < processQty; i++)
    {
        pid_t pid = fork();
        if (!pid)
        {
            childProcess(i, &chieldArguments[i]);
        }
    }
    
    for (int i = 0; i < processQty; i++)
    {
        wait(0);
    }
    
    for (int i = 0; i < processQty; i++)
    {
        free(chieldArgument[i].filenames);
    }
    
    free(chieldArguments);
    free(filenames);

    return 0;
}
