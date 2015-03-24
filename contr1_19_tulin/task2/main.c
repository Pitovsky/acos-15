#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 3 || strlen(argv[1]) != strlen(argv[2]))
    {
        printf("Wrong input\n");
    }
    
    while (1)
    {
        char string[100];
        
        char c;
        for (int i = 0; i < 99 && (c=getchar()) != '\n'; i++)
        {
            string[i] = c;
            string[i+1] = '\0';
        }
        
        for (int i = 0; i < strlen(string); i++)
        {
            char *buff;
            char symbol[2] = { *(string + i) , '\0'};
            
            if ( (buff = strstr(argv[1], symbol)) )
            {
                string[i] = argv[2][buff - argv[1]];
            }
        }
        
        printf("%s\n", string);
    }
    
    return 0;
}
