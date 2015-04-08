#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* src = getenv("URLS_SRC");
    printf("found a pipe: %s\n", src);

    FILE* srcPipe = open(src, O_RDONLY);
    if (srcPipe == NULL)
    {
        mkfifo(src, S_IFIFO | 0666);
        printf("URLS_SRC pipe created\n");
        srcPipe = open(src, "r");
    }

    char url[512];
    while (read(srcPipe, url, 512) != 0 && strcmp(url, "exit") != 0)
    {
        printf("Site: %s\n", url);
        int htmlPipe[2];
        pipe(htmlPipe);
        pid_t child = fork();
        if (child != 0)
        {
            close(htmlPipe[1]);
            char newch[1];
            int correctTag = 0;

            while (read(htmlPipe[0], newch, 1) != 0)
            {
                if (correctTag == 0 && (*newch == 'h' || *newch == 'H'))
                    ++correctTag;
                else if (correctTag == 1 && (*newch == 'r' || *newch == 'R'))
                    ++correctTag;
                else if (correctTag == 2 && (*newch == 'e' || *newch == 'E'))
                    ++correctTag;
                else if (correctTag == 3 && (*newch == 'f' || *newch == 'F'))
                    ++correctTag;
                else if (correctTag == 4)
                    correctTag = -1; //it is a href
                else if (correctTag > 0)
                    correctTag = 0; //it is not a href

                if (correctTag == -2 && *newch == '"')
                {
                    correctTag = -0; //end copy link text
                    printf("\n");
                }
                else if (correctTag == -1 && *newch == '"')
                {
                    correctTag = -2; //start copy link text
                    printf("new ref: ");
                }

                if (correctTag == -2 && *newch != '"')
                    printf("%c", *newch);
            }
            close(htmlPipe[0]);
        }
        else
        {
            close(htmlPipe[0]);
            fprintf(stderr, "starting curl...\n");
            dup2(htmlPipe[1], 1);
            close(htmlPipe[1]);
            int code = execl("/usr/bin/curl", "/usr/bin/curl", url, NULL);
            return 0;
        }
    }

    fclose(srcPipe);
    return 0;
}
