#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int NSUInteger;
typedef int NSInteger;

void readFile(char *filename, NSInteger level)
{
    NSInteger fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Can't read a file");
        return;
    }
    
    char c;
    NSInteger qty = 0;
    while (read(fd, &c, 1) != 0)
    {
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c<= '9') || (c >= 'A' && c <= 'Z') ||  c== '_'))
        {
            qty++;
        }
    }
    printf("%*s %s\t%d\n", level, "", filename, qty);
    close(fd);
}

void processDir(const char *name, NSInteger currentLevel, NSInteger level, short int sFlag, short int symbolOnce)
{
    if (level && currentLevel > level)
        return;
    
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
    {
        perror("Can't open a dir\n");
        exit(1);
    }
    
    if (!(entry = readdir(dir)))
    {
        perror("Can't read a dir");
        exit(1);
    }

    do {
        char path[1024];
        
        NSUInteger len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            
            printf("%*s[%s]\n", currentLevel*2, "", entry->d_name);
            processDir(path, currentLevel + 1, level, sFlag, symbolOnce);
        }
        else
        {
            readFile(path, currentLevel);
        }
    } while (entry = readdir(dir));
        closedir(dir);
}

int main(NSInteger argc, char **argv)
{
    char filename[1024];
    *filename = 0;
    NSInteger level = 0;
    short int sFlag = 0, symbolOnce = 0;
    
    for (NSInteger i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-r"))
        {
            level = atoi(argv[i+1]);
            i++;
        }
        
        else if (!strcmp(argv[i], "-s"))
        {
            sFlag = 1;
        }
        
        else if (!strcmp(argv[i], "-c"))
            
        {
            symbolOnce = 1;
        }
        
        else
        {
            if (strlen(filename))
            {
                perror("Wrong usage");
                exit(1);
            }
            strcpy(filename, argv[i]);
        }
        
    }
    
    if (argc > 6 || (symbolOnce && sFlag))
    {
        perror("Wrong usage");
        exit(1);
    }
    
    processDir(filename, 0, level, sFlag, symbolOnce);
    
    return 0;
}
