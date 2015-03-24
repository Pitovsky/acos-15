#include "string.h"
#include "stdio.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: usage %s word word\n", argv[0]);
        return 1;
    }
    if (strlen(argv[1]) != strlen(argv[2]))
    {
        fprintf(stderr, "Error: length is not simillar\n");
        return 2;
    }

    char ch = getchar();
    while (ch != EOF && ch != 0)
    {
        int j = 0;
        char chout = ch;
        for (j = 0; j < strlen(argv[1]); ++j)
            if (ch == argv[1][j])
                chout = argv[2][j];
        ch = getchar();
        putchar(chout);
    }

    return 0;
}

