#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const long long Prime = 53;

int main()
{
    struct Table *my_table;
    my_table = create_table(10, Prime);

    add_element(my_table, "AKOC", 1000);
    write_table(my_table);
    add_element(my_table, "bd", 1020);
    write_table(my_table);
    add_element(my_table, "COKA", 10004);
    write_table(my_table);
    add_element(my_table, "A", 10003);
    write_table(my_table);
    add_element(my_table, "mipt", 102200);
    write_table(my_table);

    remove_element(my_table, "A");
    printf("Element has been removed.\n");
    write_table(my_table);

    add_element(my_table, "Tuple", 10002);

    write_table(my_table);

    add_element(my_table, "Lol", 102);
    add_element(my_table, "123", 10009);

    write_table(my_table);

    int *rez = find_element(my_table, "F");

    printf("%d\n", (rez == NULL)? -1 : (*rez));

    dispose_table(&my_table);

    write_table(my_table);

    return 0;
}
