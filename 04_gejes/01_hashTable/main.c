#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int i = 0;
    FILE* fin = stdin;
    FILE* fout = stdout;
    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-i") == 0)
        {
            fin = fopen(argv[i + 1], "r");
            if (fin == NULL)
            {
                fprintf(stderr, "Error: cannot open this file: %s\n", argv[i + 1]);
                return 1;
            }
        }
    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-o") == 0)
        {
            fout = fopen(argv[i + 1], "w");
        }
        struct Table *ptr;
        int n = 0;
        fscanf(fin, "%d", &n);
        ptr = createTable(n);
        char com[3];
        char keys[10];
        while (strcmp(com, "end") != 0)
        {
        fscanf(fin, "%s", com);
        if (strcmp(com, "add") == 0)
        {
            fscanf(fin, "%s", keys);
            int v = 0;
            fscanf(fin, "%d", &v);
            ptr = writeElement(ptr, keys, &v);
        }
        else if (strcmp(com, "get") == 0)
        {
            fscanf(fin, "%s", keys);
            int *v;
            if (readElement(ptr, keys, &v))
                fprintf(fout, "out: %s <=> %d\n", keys, *v);
            else
                fprintf(fout, "I haven't it\n");
        }
        else if (strcmp(com, "pop") == 0)
        {
            fscanf(fin, "%s", keys);
            deleteElement(ptr, keys);
            fprintf(fout, "Ok.\n");
        }
        else if (strcmp(com, "out") == 0)
            outTable(ptr, fout);
        }
    fclose(fin);
    fclose(fout);
        clearTable(ptr);

        return 0;
}
