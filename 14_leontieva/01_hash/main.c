#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	printf("Insert size of table: ");
	int n;
	scanf ("%d", &n);
	if (n > 0)
        struct node** T = create(n);
    else {
        perror("wrong format of size");
        exit(1);
    }

	printf("insert\n");
	char buf[255];
	int data;
	//do {
        scanf("%d%s",&data, &buf );
        insert((void*)&data, buf, T, n);
    //} while (data != 0);

    printf("search");
    scanf("%s",buf);
    show_node(buf, T, n);


return 0;
}
