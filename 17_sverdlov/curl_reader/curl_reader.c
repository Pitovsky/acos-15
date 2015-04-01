#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void curl_execute(int output, const char* url_path)
{
    dup2 (output, 1);
    execlp("curl", "curl", url_path, (char*)NULL);

    perror("Can't execute curl\n");
}

void child_read(int input)
{
    // brace yourselves, ugly parsing begins

    char buf[256];
    memset(buf, 0, sizeof(buf));

    char last[7];
    last[6] = 0; // terminate string with 0

    char in_a = 0;
    char in_href = 0;

    while (1) {
        if (read(input, buf, 255)) {
            int i, j;
            char c;
            for (i = 0; buf[i]; ++i) {
                c = buf[i];
                if (last[5] == '<' && c == 'a' && !in_a) {
                    // open <a .. >
                    in_a = 1;
                    continue;
                }
                if (c == '>' && in_a) {
                    // close <a .. >
                    in_a = 0;
                    continue;
                }

                for (j = 0; j < 5; ++j) {
                    // rotate last 6 characters
                    last[j] = last[j + 1];
                }
                last[5] = c; // set last character

                if (in_a && !in_href && strcmp(last, "href=\"") == 0) {
                    // open href=" .. "
                    in_href = 1;
                    continue;
                }
                if (in_href && c == '"') {
                    // close href=" .. "
                    in_href = 0;
                    putchar('\n'); // delimit addresses by '\n'
                    continue;
                }

                // main case
                if (in_href) {
                    putchar(c);
                }
            }
        }
    }
}

int main()
{
    char* mf_name = getenv("URLS_SRC");

    int fd = open(mf_name, S_IRUSR);
    if (fd < 0) {
        printf("Can't open FIFO: %s\n", mf_name);
        return 0;
    }

    char s_url[256];
    while (1) {
        if (read(fd, s_url, 255)) {
            // recieved URL
            int fd_pipe[2];
            pipe(fd_pipe);

            char* s_url_good = strtok(s_url, "\n\r");

            pid_t pid = fork();
            if (pid == 0) {
                // child
                child_read(fd_pipe[0]);
                break;
            }

            pid = fork();
            if (pid == 0) {
                // child
                curl_execute(fd_pipe[1], s_url_good);
                break;
            }

        }
    }

    return 0;
}
