#include "myfio.h"

static int BUF_SIZE = 10;

myFile* myfopen(const char* filename, const char* mode)
{
    int flag = 0;
    int i;
    int modelen = strlen(mode);

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
    }

    myFile* newFileStruct = (myFile*)malloc(sizeof(myFile));
    newFileStruct->buf = (char*)malloc(sizeof(char)*BUF_SIZE);
    newFileStruct->fd = open(filename, flag, 0666);
    if (newFileStruct->fd < 0)
        return NULL;
    newFileStruct->occlen = 0;
    return newFileStruct;
}

int myfwrite(myFile* file, const char* input, int len)
{
    int total = 0;
    while (total < len)
    {
        int i = total;
        while (i < len && file->occlen < BUF_SIZE)
        {
            file->buf[file->occlen] = input[i];
            ++i;
            ++(file->occlen);
        }
        if (i < len)
        {
            int writtenBytes = write(file->fd, file->buf, BUF_SIZE);
            if (writtenBytes <= 0)
                return total + writtenBytes;
            else
            {
                total = total + writtenBytes;
                int j;
                for (j = writtenBytes; j < BUF_SIZE; ++j) //if not all bytes was written
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
