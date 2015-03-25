#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

int countWords(const char* filePath)
{
    FILE* inputFile = fopen(filePath, "r");
    if (!inputFile)
        return -1;

    int answer = 0;
    fscanf(inputFile, "%*[^a-zA-Z0-9_]");

    while(!feof(inputFile) && ++answer)
    {
        fscanf(inputFile, "%*[a-zA-Z0-9_]");
        fscanf(inputFile, "%*[^a-zA-Z0-9_]");
    }

    fclose(inputFile);

    return answer;
}

void printWordStats(const char* filePath, char followSymlinks, int recursionDepthLimit)
{
    if (recursionDepthLimit)
    {
        struct stat sb;
        if (followSymlinks)
            stat(filePath, &sb);
        else
            lstat(filePath, &sb);

        if (sb.st_mode & S_IFREG)
        {
            int count = countWords(filePath);
            if (count != -1)
                printf("file %s has %i words\n", filePath, count);
            else
                fprintf(stderr, "file %s can not be opened\n", filePath);
        }
        else if (sb.st_mode & S_IFDIR)
        {
            printf("%s:\n", filePath);
            size_t baseFileLength = strlen(filePath);
            char* nextFileName = (char*) malloc(baseFileLength + 258);
            memcpy(nextFileName, filePath, baseFileLength + 1);
            nextFileName[baseFileLength] = '/';
            nextFileName[baseFileLength + 1] = 0;

            struct dirent *pDirent;
            DIR *pDir;

            pDir = opendir(filePath);
            assert(pDir);

            while (pDirent = readdir(pDir))
            {
                if (pDirent->d_name[0] != '.'
                   || (pDirent->d_name[0] == '.' && pDirent->d_name[1] != 0 &&
                       (pDirent->d_name[1] != '.' || pDirent->d_name[2] != 0)))
                {
                    nextFileName[baseFileLength + 1] = 0;
                    strcat(nextFileName + baseFileLength, pDirent->d_name);
                    printWordStats(nextFileName, followSymlinks, recursionDepthLimit - 1);
                }
            }
            free(nextFileName);
            closedir (pDir);
        }
    }
}

int main (int argc, char **argv)
{
    int followSymlinks = 1, recursionDepth = 0;
    opterr = 0;

    int characterOption = -1;
    while ((characterOption = getopt(argc, argv, "sr:")) != -1)
    {
        switch (characterOption)
        {
        case 's':
            followSymlinks = 0;
            break;
        case 'r':
            recursionDepth = atoi(optarg);
            break;
        case '?':
            if (optopt == 'r')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort ();
        }
    }

    if (optind + 1 != argc)
    {
        fprintf(stderr, "Too many arguments, only 1 filename needed\n");
        return 2;
    }
    if (0 == recursionDepth)
        recursionDepth = -1;
    printWordStats(argv[argc - 1], followSymlinks, recursionDepth);
    return 0;
}

