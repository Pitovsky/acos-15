#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
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

char* visitedLinks[1000];
int visitedLinkCount = 0;

int outWordDir(char* unabpath, int recDeep, int jumpLink)
{
    int i = 0;
    char word[262143];
    char path[256];
    char entryPath[256];
    realpath(unabpath, path);
    strcpy(entryPath, path);
    struct dirent** namelist;
    int inDirCount = scandir(".", &namelist, NULL, alphasort);
    printf("Now in directory: %s, else %d levels\n", path, recDeep);
    for (i = 0; i < inDirCount; ++i)
        if (strcmp(namelist[i]->d_name, "..") != 0 && strcmp(namelist[i]->d_name, ".") !=0)
    {
        int wordCount = 0;
        if (recDeep != 0 && chdir(namelist[i]->d_name) == 0)
        {
            getcwd(path, sizeof(path));
            outWordDir(path, recDeep - 1, jumpLink);
            chdir(entryPath);
            strcpy(path, entryPath);
            //realpath(unabpath, path);
            printf("Return to directory: %s, else %d levels\n", path, recDeep);
        }
        else
        {
            struct stat st;
            lstat(namelist[i]->d_name,&st);
            if (S_ISLNK(st.st_mode))
            {
                char realPath[256];
                realpath(namelist[i]->d_name, realPath);
                printf("%s - symlink to %s\n", namelist[i]->d_name, realPath);
                if (jumpLink == 0)
                    continue;
                if (jumpLink == -1)
                {
                    int j;
                    int visited = 0;
                    for (j = 0; j < visitedLinkCount; ++j)
                        if (strcmp(visitedLinks[j], realPath) == 0)
                            ++visited;
                    if (visited != 0)
                    {
                        printf("and I visited it, see upper\n");
                        continue;
                    }
                    else
                    {
                        visitedLinks[visitedLinkCount] = (char*)malloc(strlen(realPath)*sizeof(char));
                        strcpy(visitedLinks[visitedLinkCount], realPath);
                        ++visitedLinkCount;
                    }
                }
            }
            FILE* fin = fopen(namelist[i]->d_name, "r");
            if (fin == NULL)
            {
                fprintf(stderr, "Error open this file: %s\n", namelist[i]->d_name);
                continue;
            }
            while (fscanf(fin, "%s", word) == 1)
                wordCount++;
            fclose(fin);
            printf("%s:\t%d (recoursion else: %d levels)\n", namelist[i]->d_name, wordCount, recDeep);
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    char path[255];
    int recDeep = 0;
    int jumpLink = 1;
    memset(path, 0, 255);
    strcpy(path, ".");
    int i = 0;
    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-f") == 0)
            strcpy(path, argv[i + 1]);

    for (i = 0; i < argc - 1; ++i)
        if (strcmp(argv[i], "-r") == 0)
            recDeep = strToInt(argv[i + 1]) - 1;

    for (i = 0; i < argc; ++i)
        if (strcmp(argv[i], "-c") == 0)
            jumpLink = -1;

    for (i = 0; i < argc; ++i)
        if (strcmp(argv[i], "-s") == 0)
            jumpLink = 0;

    chdir(path);
    printf("Ok, I will try it. %d recoursion levels?\n", recDeep);
    outWordDir(path, recDeep, jumpLink);

    for (i = 0; i < visitedLinkCount; ++i)
        free(visitedLinks[i]);

    return 0;
}
