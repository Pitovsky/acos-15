#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 3) {
        printf("Error\n");
        return -1;
    }

    char *s_find = argv[1];
    char *s_replace = argv[2];

    char *s_in = (char*)malloc(sizeof(char) * 256);//строка, полученная из потока ввода
    char *tmp = (char*)malloc(sizeof(char) * 256);//временная строка
    char *entr;
    int i;

    char *s_in_tmp;
    while(1)
    {
        gets(s_in);
        s_in_tmp = s_in;

        while(strpbrk(s_find, s_in_tmp) !=NULL) {
            entr = strpbrk(s_in_tmp, s_find);
            strcpy(tmp, entr + 1);
            //найдём индекс элемента, который нужно вставить
            i = 0;
            while(s_find[i] != *entr) {i++;}
            strcpy(entr, &s_replace[i]);//здесь мне нужно ещё найти, какой элемент вставлять
            strcpy(entr + 1, tmp);
            s_in_tmp = entr + 1;
        }

        printf("%s\n", s_in);
    }

    return 0;
}
