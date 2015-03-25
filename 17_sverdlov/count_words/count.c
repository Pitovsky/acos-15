#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

static int counter;

char is_word_char(char c)
{
    return ('0' <= c && c <= '9') || 
           ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           c == '_';
}

void count_words(const char *path)
{
    FILE* fp = fopen(path, "r");

    if (!fp) {
        fprintf(stderr, "Can't open %s file\n", path);
        return;
    }

    char c;
    char last_word = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (is_word_char(c)) {
            if (!last_word) {
                ++counter;
            }
        }
        last_word = is_word_char(c);
    }

    fclose(fp);
}

static void list_dir(const char *path, 
                     int dep, 
                     const int MAX_DEP, 
                     const char LINKS_ALLOWED)
{
    DIR *d;

    d = opendir(path);
    if (!d) {
        fprintf(stderr, "Can't open %s directory\n", path);
        closedir(d);
        return;
    }
    /*printf("Entered %s directory\n", path);*/

    while (1) {
        struct dirent *entry;
        const char *name;
        char to_path[1000];

        entry = readdir(d);
        if (!entry) {
            break;
        }

        name = entry->d_name;
        sprintf(to_path, "%s/%s", path, name);

        if (entry->d_type & DT_DIR) {

            if (strcmp(name, "..") != 0 && strcmp(name, ".") != 0) {
                if (!MAX_DEP || dep + 1 < MAX_DEP) {
                    list_dir(to_path, dep + 1, MAX_DEP, LINKS_ALLOWED);
                }
            }
        } else if ((entry->d_type & DT_REG) || 
                   (LINKS_ALLOWED && (entry->d_type & DT_LNK))) {
            count_words(to_path);
        }
    }

    int failure = closedir(d);
    if (failure) {
        fprintf(stderr, "Cant' close %s directory\n", path);
        return;
    }
}


int main(int argc, char **argv)
{
    char* path;
    int i;
    int MAX_DEP;
    int LINKS_ALLOWED;

    if (argc < 2) {
        printf("Usage:\ncount [path]\n");
        return 0;
    }

    path = argv[1];
    for (i = 2; i < argc;) {
        if (strcmp(argv[i], "-r") == 0) {
            if (i == argc) {
                printf("Must be argument after -r\n");
                return 0;
            }
            if (sscanf(argv[i + 1], "%d", &MAX_DEP) == 1) {
                i += 2;
            } else {
                printf("Argument after -r must be integer\n");
                return 0;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            LINKS_ALLOWED = 1;
            i += 1;
        } else {
            printf("Unknown symbol %s, quitting\n", argv[i]);
            return 0;
        }
    }

    counter = 0;
    list_dir(path, 0, MAX_DEP, LINKS_ALLOWED);
    printf("Number of words in directories of %s is %d\n", argv[1], counter);

    return 0;
}
