#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int strToInt(char* str)
{
    int i = 0;
    int ret = 0;
    for (i = 0; i < strlen(str); ++i)
    {
        ret = ret*10 + (str[i] - '0');
    }
    return ret;
}

struct dirent** namelist;

int outWordDir(char* path, int recDeep)
{
    int i = 0;

    int inDirCount = scandir(".", &namelist, NULL, alphasort);
    getcwd(path, sizeof(path));
    printf("Now in directory: %s, else %d levels\n", path, recDeep);
    for (i = 0; i < inDirCount; ++i)
        if (strcmp(namelist[i]->d_name, "..") != 0 && strcmp(namelist[i]->d_name, ".") != 0)
    {
        int wordCount = 0;
        if (recDeep > 0 && chdir(namelist[i]->d_name) == 0)
        {
            getcwd(path, sizeof(path));
            outWordDir(path, recDeep - 1);
            chdir("..");
            getcwd(path, sizeof(path));
            printf("Done.\n");
        }
        else
        {
            FILE* fin = fopen(namelist[i]->d_name, "r");
            if (fin == NULL)
            {
                fprintf(stderr, "Error open this file: %s\n", namelist[i]->d_name);
                continue;
            }
            char word[150];

            while (fscanf(fin, "%s", word) == 1)
                wordCount++;
            //free(word);
            fclose(fin);
            printf("\t%s:\t%d (recourse else: %d levels)\n", namelist[i]->d_name, wordCount, recDeep);
        }
    }
    //free(word);
    printf("Directory done.\n");
    return 0;
}

int main(int argc, char** argv)
{
    char path[255];
    int recDeep = 0;
    memset(path, 0, 255);
    strcpy(path, ".");
    int i = 0;
    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-f") == 0)
            strcpy(path, argv[i + 1]);

    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-r") == 0)
            recDeep = strToInt(argv[i + 1]);

    chdir(path);
    printf("Ok, I will try it. %d recoursive levels?\n", recDeep);
    getcwd(path, sizeof(path));
    outWordDir(path, recDeep);

    return 0;
}
