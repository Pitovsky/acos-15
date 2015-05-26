#ifndef MYFIO_H_INCLUDED
#define MYFIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct
{
    int fd;
    char* buf;
    int occlen;
} myFile;

myFile* myfopen(const char* filename, const char* mode);

int myfwrite(myFile* file, const char* input, int len);
int myfread(myFile* file, char* output, int len);

int myfclose(myFile* file);

#endif // MYFIO_H_INCLUDED
