#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	struct Table *ptr;
	int n = 0;
	int v = 0;
	scanf("%d", &n);
	ptr = createTable(n);
	char com[3];
	char keys[10];
	while (strcmp(com, "end") != 0)
	{
        scanf("%s", com);
        if (strcmp(com, "add") == 0)
        {
            scanf("%s", keys);
            scanf("%d", &v);
            ptr = writeElement(ptr, keys, v);
        }
        else if (strcmp(com, "get") == 0)
        {
            scanf("%s", keys);
            if (readElement(ptr, keys, &v))
                printf("out: %s <=> %d\n", keys, v);
            else
                printf("I haven't it\n");
        }
        else if (strcmp(com, "pop") == 0)
        {
            scanf("%s", keys);
            deleteElement(ptr, keys);
            printf("Ok.\n");
        }
        else if (strcmp(com, "out") == 0)
            outTable(ptr);
	}

	clearTable(ptr);

	return 0;
}
