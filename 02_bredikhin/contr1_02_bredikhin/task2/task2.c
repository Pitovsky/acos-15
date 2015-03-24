#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char SuperSwap(char c, char* str1, char* str2)
{
	int i, length;
	length = strlen(str1);

	for (i = 0; i < length; i++)
	{
		if (c == str1[i])
			return str2[i];
	}
	return c;
}

int main(int argc, char* argv[])
{
	char c;
	int length = strlen(argv[1]);
	char* str1 = (char*)malloc(length);
	char* str2 = (char*)malloc(length);

	str1 = argv[1];
	str2 = argv[2];

	while ((c = getchar()) != EOF)
	{
		printf("%c", SuperSwap(c, str1, str2));
	}
	return 0;
}