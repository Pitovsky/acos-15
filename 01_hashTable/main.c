#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "hash.h"

int main(int argc, char **argv)
{
    
    Hashtable *hashtable = createHashtable(65536);
    
    FILE *fd = fopen("./input.txt", "r");
    if (!fd)
    {
        perror("Can't read a file\n");
        exit(1);
    }
    
    char command[2];
    while ((fscanf(fd, "%s", command))!=EOF)
    {
        if (*command == 'a')
        {
            char value[32];
            char key[32];
            fscanf(fd, "%s %s", value, key);
            setValue(hashtable, key, value);
            
            printf("Set value %s For key : %s\n", value, key);
        }
        else if (*command == 'f')
        {
            char key[32];
            fscanf(fd, "%s", key);
            printf("Got value %s For key : %s\n", getValue(hashtable, key), key);
        }
        else if (*command == 'r')
        {
            char key[32];
            fscanf(fd, "%s", key);
            printf("Delete value %s For key : %s\n", getValue(hashtable, key), key);
            deleteValue(hashtable, key);
        }
    }
    
    fclose(fd);
    releaseHashtable(hashtable);
    
    return 0;
}