#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const long long PRIME = 53;
const int INPUT_STRING_SIZE = 200;

int main()
{
    struct Table *my_table;
    FILE* f = fopen("input.txt", "r");

    if (f == NULL)
        return 1;
    int table_size;
    fscanf(f, "%d", &table_size);

    my_table = create_table(table_size, PRIME);

    char *input_str = malloc(INPUT_STRING_SIZE);

    int read_code = 0;
    while (read_code != EOF)
    {
        char code[10];
        int input_value;
        read_code = fscanf(f, "%s", code);

        if (read_code == EOF)
            break;

        if (read_code != 1)
        {
            dispose_table(&my_table);
            free(input_str);
            fclose(f);

            return 1;
        }

        switch (*code)
        {
            case 'a':
                read_code = fscanf(f, "%d %s", &input_value, input_str);

                if (read_code != 2)
                    return 1;
                add_element(my_table, input_str, input_value);
                printf("\033[32mADD %s %d\n\n\033[0m", input_str, input_value);
                break;
            case 'f':
                read_code = fscanf(f, "%s", input_str);

                if (read_code != 1)
                    return 1;

                int *rez = find_element(my_table, input_str);
                if (rez == 0)
                    printf("\033[32m%s NOT FOUND\n\n\033[0m", input_str);
                else printf("\033[32mKEY: %s - VALUE: %d\n\n\033[0m", input_str,*rez);

                break;
            case 'r':
                read_code = fscanf(f, "%s", input_str);

                if (read_code != 1)
                    return 1;

                remove_element(my_table, input_str);
                printf("\033[32mTRY TO REMOVE: %s\n\n\033[0m", input_str);
                break;
         }
         printf("----------------------------\n");
         write_table(my_table);
         printf("----------------------------\n\n");
    }

    dispose_table(&my_table);
    free(input_str);
    fclose(f);
    return 0;
}
