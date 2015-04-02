#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

char **readStringsFromDescriptor(FILE *fd)
{
    char **string = (char **)malloc(sizeof(char *));
    char c = 0;
    size_t stringsqty = 0;
    for (size_t i = 0; (c = getc(fd)) != EOF && c != '\0'; i++)
    {
        if (i == 0)
        {
            string = (char **)realloc(string, (stringsqty + 1) * sizeof(char *));
            string[stringsqty] = (char *)malloc(sizeof(char));
            *string[stringsqty] = '\0';
        }
        
        if (c == '\n')
        {
            i = -1;
            stringsqty++;
        }
        else
        {
            string[stringsqty] = (char *)realloc(string[stringsqty], i + 2);
            string[stringsqty][i] = c;
            string[stringsqty][i+1] = '\0';
        }
    }
    string = (char **)realloc(string, (stringsqty + 1) * sizeof(char *));
    string[stringsqty] = (char *)malloc(sizeof(char));
    string[stringsqty][0] = '\0';
    return string;
}

char *stringFromDescriptor(int fd)
{
    char *string = (char *)malloc(sizeof(char));
    *string = '\0';
    
    char c = 0;
    int res = 1;
    for (size_t i = 0; res; i++)
    {
        if ((res = read(fd, &c, 1)) == 0)
        {
            break;
        }
        else if (res < 0)
        {
            break;
        }
        else
        {
            string = (char *)realloc(string, i + 2);
            string[i] = c;
            string[i+1] = '\0';
        }
    }
    return string;
}

void curl(char **argv)
{
    if(execv(*argv, argv))
    {
        perror("Exec issue");
        exit(1);
    }
    exit(1);
}

void refsPrinter(char *src)
{
    char *ptr = src;
    while (ptr)
    {
        ptr = strstr(ptr, "<a href=\"http");
        size_t diff = strstr(ptr + strlen("<a href=\""), "\"") - strlen("<a href=\"") + 1 - ptr;
        char *URL = ptr + strlen("<a href=\"");
        URL[diff - 1] = '\0';
        printf("%s\n", URL);
        ptr = URL + diff;
    }
}

int main()
{
    char *enviromentVariableName = getenv("URLS_SRC");
    if (enviromentVariableName == NULL)
    {
        perror("Can't get enviroment variable");
        exit(1);
    }
    printf("USING NAMED PIPE : %s\n", enviromentVariableName);
    
    FILE *namedPipeFD = 0;
    if ((namedPipeFD = fopen(enviromentVariableName, "r")) < 0)
    {
        perror("Can't open a named pipe");
        exit(1);
    }
    printf("ESTABLISHED FILE DESCRIPTOR WITH NAMED PIPE\n");
    
    char **sitesToParse = readStringsFromDescriptor(namedPipeFD);
    
    int parsedSitePipe[2];
    pipe(parsedSitePipe);
    
    fcntl(parsedSitePipe[0], F_SETFL, O_NONBLOCK);
    fcntl(parsedSitePipe[1], F_SETFL, O_NONBLOCK);
    
    char **argv = (char **)malloc(sizeof(char *) * 4);
    
    argv[0] = (char *)malloc(sizeof(char) * (strlen("/usr/bin/curl") + 1));
    **argv = '\0';
    strcpy(argv[0], "/usr/bin/curl");
    argv[2] = (char *)malloc(sizeof(char) * (strlen("--silent") + 1));
    argv[2][0] = '\0';
    strcat(argv[2], "--silent");
    argv[3] = NULL;
    
    for (size_t i = 0; sitesToParse[i][0]; i++)
    {
        printf("PARSING: %s\n", sitesToParse[i]);
        argv[1] = sitesToParse[i];
        if (fork() == 0)
        {
            if(dup2(parsedSitePipe[1], STDOUT_FILENO) < 0)
            {
                perror("Dup wrong result");
                exit(1);
            }
            curl(argv);
            exit(1);
        }
        
        wait(0);
        
        if (fork() == 0)
        {
            char *src = stringFromDescriptor(parsedSitePipe[0]);
            refsPrinter(src);
            free(src);
            exit(1);
        }
        wait(0);
        
        free(sitesToParse[i]);
    }
    
    
    fclose(namedPipeFD);
    
    return 0;
}
