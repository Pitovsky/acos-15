#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("%s", "Incorrect number of arguments");
		return 1;
	}
	
	if (strlen(argv[1]) != strlen(argv[2]))
	{
		printf("%s", "Lengths of arguments are not equal");
		return 1;
	}
	
	int table[256];
	int v;
	
	for (v = 0; v < 256; v++)
		table[v] = 0;
	
	for (v = 0; v < strlen(argv[1]); v++)
		table[argv[1][v]] = argv[2][v];
	char c = getchar();
	while (c != '\n')
	{
		if (table[c] != 0)
			printf("%c", table[c]);
		else
			printf("%c", c);
		c = getchar();
	}
	
	return 0;
}
