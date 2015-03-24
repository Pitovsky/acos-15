#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("too few arguments\n");
        return 0;
    }

    char convert[256];
    memset(convert, 0, sizeof(convert));
    size_t i;
    for (i = 0; argv[1][i]; ++i) {
        convert[argv[1][i]] = argv[2][i];
    }

    char c;
    while ((c = fgetc(stdin)) != EOF) {
        char to = (convert[c] ? convert[c] : c);
        printf("%c", to);
    }
    printf("exiting");


    return 0;
}
