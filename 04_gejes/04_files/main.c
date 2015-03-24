#include <stdio.h>
#include <unistd.h>
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
int outWordDir(char* path, int recDeep)
{
    int i = 0;
    char word[1023];
    struct dirent** namelist;
    int inDirCount = scandir(".", &namelist, NULL, alphasort);
    printf("Now in directory: %s, else %d levels\n", path, recDeep);
    for (i = 0; i < inDirCount; ++i)
        if (strcmp(namelist[i]->d_name, "..") != 0 && strcmp(namelist[i]->d_name, ".") !=0)
    {
        int wordCount = 0;
        if (recDeep > 0 && chdir(namelist[i]->d_name) == 0)
        {
            getcwd(path, sizeof(path));
            outWordDir(path, recDeep - 1);
            chdir("..");
            getcwd(path, sizeof(path));
        }
        else if (recDeep <= 0)
        {
            printf("%s/%s:\tit's a dir (but recourse ended)\n", path, namelist[i]->d_name);
        }
        else
        {
            FILE* fin = fopen(namelist[i]->d_name, "r");
            if (fin == NULL)
            {
                fprintf(stderr, "Error open this file: %s\n", namelist[i]->d_name);
                continue;
            }
            while (fscanf(fin, "%s", word) == 1)
                wordCount++;
            fclose(fin);
            printf("%s/%s:\t%d (recourse else: %d levels)\n", path, namelist[i]->d_name, wordCount, recDeep);
        }
    }
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
    outWordDir(path, recDeep);

    return 0;
}
