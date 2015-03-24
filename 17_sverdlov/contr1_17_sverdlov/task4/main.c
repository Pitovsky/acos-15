#include <stdio.h>



int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("wrong input format, must be 2 arguments\n");
        return 0;
    }

    int cmds[2];
    pipe(cmds);


    return 0;
}
