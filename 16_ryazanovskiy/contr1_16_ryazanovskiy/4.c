#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv)
{
    assert(3 == argc);

    FILE* pipe1 = popen(argv[1], "r");
    FILE* pipe2 = popen(argv[2], "r");

    char line1[8192] = "", line2[8192] = "";

    while (!feof(pipe1) || !feof(pipe2))
    {
        line1[0] = line2[0] = 0;
        fscanf(pipe1, "%8190[^\n]\n", line1);
        fscanf(pipe2, "%8190[^\n]\n", line2);
        printf("%s\t%s\n", line1, line2);
    }

    return 0;
}
