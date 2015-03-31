#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* src = getenv("URLS_SRC");
    printf("finded pipe: %s\n", src);

    FILE* srcPipe = open(src, O_RDONLY);
    if (srcPipe == NULL)
    {
        mkfifo(src, S_IFIFO | 0666);
        printf("URLS_SRC pipe created\n");
        srcPipe = open(src, "r");
    }

    char url[512];
    while (read(srcPipe, url, 512) != 0)
    {
        printf("Site: %s\n", url);
        int htmlPipe[2];
        pipe(htmlPipe);
        pid_t child = fork();
        if (child != 0)
        {
            char htmlCode[1024];
            close(htmlPipe[1]);
            printf("HTML CODE FOR IT:\n");
            while (read(htmlPipe[0], htmlCode, 1024) != 0)
            {
                //printf(":%s\n", htmlCode);
                int i = 4;
                while (i < 512 && htmlCode[i] != 0)
                {
                    if ((htmlCode[i - 3] == 'h' || htmlCode[i - 3] == 'H') &&
                        (htmlCode[i - 2] == 'r' || htmlCode[i - 2] == 'R') &&
                        (htmlCode[i - 1] == 'e' || htmlCode[i - 1] == 'E') &&
                        (htmlCode[i] == 'f' || htmlCode[i] == 'F'))
                    {
                        printf("nef ref: ");
                        while (htmlCode[i] != '"')
                            ++i;
                        ++i;
                        while (htmlCode[i] != '"')
                        {
                            printf("%c", htmlCode[i]);
                            ++i;
                        }
                        printf("\n");
                    }
                    ++i;
                }
            }
            close(htmlPipe[0]);
        }
        else
        {
            close(htmlPipe[0]);
            printf("starting curl...\n");
            dup2(htmlPipe[1], 1);
            close(htmlPipe[1]);
            int code = execl("/usr/bin/curl", "/usr/bin/curl", url, NULL);
            return 0;
        }
    }

    fclose(srcPipe);
    return 0;
}
