#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
        if ((argc < 3)||(argc >3))
        {
                printf("неправильное число аргументов\n");
        }
        else
        {
                char input;
                int i, key, j;
                while (!feof(stdin))
                {
                        input = getc(stdin);
                        i = 0;
                        key = 0;
                        while ((argv[1][i] != 0)&&(key!=1))
                        {
                                if (argv[1][i] == input)
                                {
                                        printf("%c\n",argv[2][i]);
                                        key = 1;
                                }
                                ++i;
                        }
                }
        }
        return 0;
}
