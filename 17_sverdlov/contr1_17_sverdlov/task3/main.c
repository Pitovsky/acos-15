#include <stdio.h>
#include <unistd.h>

char is_word(char c) {
    return (c == '-' || c == '_' || ('0' <= c && c <= '9') || ('a' <= c && c <= 'z'));
}

void process_file(char* path, pid_t pid) 
{
    printf("%s proccessed by %d\n", path, pid);
    FILE* fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("error opening file %s\n", path);
        return;
    }

    char c;
    int n_words = 0;
    char last_word = 0;
    while (1) {
        c = fgetc(fp);
        if (c == EOF) {
            break;
        }
        if (is_word(c)) {
            if (!last_word) {
                ++n_words;
            }
        }
        last_word = is_word(c);
    }
    printf("%d words in %s processed by %d\n", n_words, path, pid);
    _exit(0);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("too few arguments");
        return 0;
    }

    int n_processes;
    sscanf(argv[argc - 1], "%d", &n_processes);
    printf("n_processes: %d\n", n_processes);

    pid_t msg[2];
    pipe(msg);

    pid_t pid;
    int i, j;
    for (j = 0; j < n_processes; ++j) {
        if (j + 2 >= argc) {
            // no tasks for j
            break;
        }
        pid = fork();
        if (pid < 0) {
            printf("fork() FAILED\n");
        } else if (pid) {
            // child

            // write pid
            write(msg[1], &pid, sizeof(pid));
            for (i = 1 + j; i + 1 < argc; i += n_processes) {
                process_file(argv[i], pid);
            }
            break;
        }
    }
    printf("root now waits:(\n");

    // only main program here (because of _exit(0) in process_file()
    while (1) {
        int retval = read(msg[0], &pid, sizeof(pid));
        if (retval == EOF) {
            break;
        }
        printf("wait for pid: %d\n", (int)pid);
        waitpid(pid);
    }
    printf("now i'm done\n");

    return 0;
}
