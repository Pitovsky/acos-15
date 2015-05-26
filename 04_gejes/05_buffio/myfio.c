#include "myfio.h"

static int defaultBufSize = 1024; //default buffer size

void setFileBufSize(int newSize)
{
    defaultBufSize = newSize;
}

myFile* myfopen(const char* filename, const char* mode)
{
    int flag = 0;
    int i;
    int modelen = strlen(mode);
    int bufSize = defaultBufSize;

    for (i = 0; i < modelen; ++i)
    {
        if (mode[i] == 'w')
            flag = flag | O_WRONLY | O_CREAT;
        else if (mode[i] == 't')
            flag = flag | O_TRUNC;
        else if (mode[i] == 'r')
            flag = flag | O_RDONLY;
        else if (mode[i] == 'a')
            flag = flag | O_APPEND;
        else if (mode[i] == 'S')
            sscanf(mode + i + 1, "%d", &bufSize);
    }

    myFile* newFileStruct = (myFile*)malloc(sizeof(myFile));
    newFileStruct->fd = open(filename, flag, 0666);
    if (newFileStruct->fd < 0)
    {
        free(newFileStruct);
        return NULL;
    }
    newFileStruct->occlen = 0;
    newFileStruct->buf = (char*)malloc(sizeof(char)*bufSize);
    newFileStruct->bufSize = bufSize;
    return newFileStruct;
}

int myfwrite(myFile* file, const char* input, int len)
{
    int total = 0;
    while (total < len)
    {
        int i = total;
        while (i < len && file->occlen < file->bufSize)
        {
            file->buf[file->occlen] = input[i];
            ++i;
            ++(file->occlen);
        }
        if (i < len)
        {
            int writtenBytes = write(file->fd, file->buf, file->bufSize);
            if (writtenBytes <= 0)
                return total + writtenBytes;
            else
            {
                total = total + writtenBytes;
                int j;
                for (j = writtenBytes; j < file->bufSize; ++j) //if not all bytes was written
                    file->buf[j - writtenBytes] = file->buf[j];
                file->occlen = file->occlen - writtenBytes;
            }
        }
        else
            total = i;
    }
    return total;
}

int myfread(myFile* file, char* output, int len)
{
    int total = 0;
    while (total < len) //if not all bytes was recieved
    {
        int recBytes = read(file->fd, output + total, len);
        if (recBytes <= 0)
            return total;
        total = total + recBytes;
    }
    return total;
}

int myfclose(myFile* file)
{
    int i = 0;
    while (i < file->occlen)
    {
        int writtenBytes = write(file->fd, file->buf + i, file->occlen - i);
        if (writtenBytes < 0)
            return -1;
        i = i + writtenBytes;
    }
    if (close(file->fd) < 0)
        return -1;
    free(file->buf);
    free(file);
    return 0;
}
