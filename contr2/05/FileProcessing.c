#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int NSUInteger;
typedef int NSInteger;

unsigned long co(const char *name)
{
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
    
    unsigned long qty = 0;
    
    do
    {
        char path[1024];
        
        NSUInteger len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            qty += co(path);
        }
        else if (entry->d_type == DT_LNK || entry->d_type == DT_REG)
        {
            if (strncmp(entry->d_name, ".", 1) == 0)
                continue;
            
            qty++;
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
    
    return qty;
}

unsigned long cs(const char *name)
{
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
    
    unsigned long size = 0;
    
    do
    {
        char path[1024];
        
        NSUInteger len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            size += cs(path);
        }
        else if (entry->d_type == DT_LNK || entry->d_type == DT_REG)
        {
            if (strncmp(entry->d_name, ".", 1) == 0)
                continue;
            
            FILE *fd;
            if ((fd = fopen(path, "r")) == NULL)
            {
                perror("Can't open a file");
                exit(1);
            }
            
            fseek(fd, 0L, SEEK_END);
            size += ftell(fd);
            
            fclose(fd);
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
    
    return size;
}