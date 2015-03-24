#include <assert.h>
#include <string.h>

#include <stdio.h>

int main(int argc, char** argv)
{
    assert(3 == argc);
    char *from = argv[1], *to = argv[2];

    size_t length = strlen(from);
    assert(strlen(to) == length);

    char replacement_map[256] = "";
    for (size_t i = 0; i < length; i++)
        replacement_map[from[i]] = to[i];

    while (!feof(stdin))
    {
        char c = getchar();
        putchar(replacement_map[c] ? replacement_map[c] : c);
    }

    return 0;
}
