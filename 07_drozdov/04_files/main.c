#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

int count_words(FILE* file)
{
    char cur_symb;
    int words = 0;
    int word_is_read = 0;

    while (cur_symb = getc(file))
    {
        if (((cur_symb >= 'a') && (cur_symb <='z'))
             || ((cur_symb >= 'A') && (cur_symb <= 'Z'))
             || ((cur_symb >= '0') && (cur_symb <= '9'))
             || (cur_symb == '_'))
            word_is_read = 0;
        else
        {
            if (!word_is_read)
                words++;
            word_is_read = 1;
        }
    }

    return words;
}

void process(char* cur_dir, int cur_depth, int depth, int links)
{
    if (cur_depth > depth)
        return;

    DIR* cur_directory = mkdir(cur_dir);
    struct dirent* cur_elem;
    cur_elem = readdir(cur_directory);

    while (cur_elem != NULL)
    {
        if ((strcmp(cur_elem->d_name, ".") == 0) || (strcmp(cur_elem->d_name, "..") == 0))
        {
            cur_elem = readdir(cur_directory);
            continue;
        }

        char path[MAX_PATH];
		strcpy(path, cur_dir);
		strcat(path, "/");
		strcat(path, cur_elem->d_name);

		struct stat info;
		lstat(path, &info);

		if (S_ISREG(info.st_mode))
        {
            FILE* cur_file = fopen(path, "r");
            printf("%s %d\n", path, count_words(cur_file));
            fclose(cur_file);
        }
        if (S_ISDIR(info.st_mode))
            process(path, cur_depth + 1, depth, links);
        if ((S_ISLNK(info.st_mode)) && (links))
        {
            int n = readlink(path, path, sizeof(path) - 1);
            path[n] = '\0';
            FILE* cur_file = fopen(path, "r");
            printf("%s %d\n", path, count_words(cur_file));
            fclose(cur_file);
        }

        cur_elem = readdir(cur_directory);
    }
}

int main(int argc, char* argv[])
{
    int depth;
    int links = 1;

    if (strcmp(argv[3], "0") == 0)
        depth = INT_MAX;
    else
        depth = atoi(argv[3]);
    if (argc == 5)
        links = 0;
    process(argv[1], 1, depth, links);
    return 0;
}
