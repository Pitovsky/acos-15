#include <assert.h>
#include <string.h>

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            char* temp = argv[argc - 1];
            argv[argc - 1] = argv[i];
            argv[i] = temp;
            break;
        }
    }

    if ('-' != argv[argc - 1][0] || 'n' != argv[argc - 1][1])
    {
        fprintf(stderr, "number of processes is not specified\n");
        fprintf(stderr, "example usage: ./3 -n2 1.c 2.c\n");
        return 1;
    }

    const int processCount = atoi(argv[argc - 1] + 2);

    assert(processCount > 0);

    int currentFile = 1;
    while (currentFile < argc - 1)
    {
        int currentProcess = 0;
        for (;
            currentFile < argc - 1 && currentProcess < processCount;
            currentProcess++, currentFile++)
        {
            if (!fork())
            {
                FILE* inputFile = fopen(argv[currentFile], "r");
                assert(inputFile);
                int answer = 0;
                fscanf(inputFile, "%*[^QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890_-]");

                while(!feof(inputFile) && ++answer)
                {
                    fscanf(inputFile, "%*[QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890_-]");
                    fscanf(inputFile, "%*[^QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890_-]");
                }

                fclose(inputFile);
                printf("%s\t%i\n", argv[currentFile], answer);
                return 0;
            }
        }

        for (int i = 0; i < currentProcess; i++)
            wait();
    }

    return 0;
}
