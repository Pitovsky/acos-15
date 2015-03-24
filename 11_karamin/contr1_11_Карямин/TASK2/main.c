#include <stdio.h>
//#pragma warning (disable:4996)


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("There aren't 2 parameters!");
		return 1;
	}

	char
		*in = (char*)malloc(100 * sizeof(char)),
		*out = (char*)malloc(100 * sizeof(char)),
		*s1, *s2;
	
	s1 = argv[1];
	s2 = argv[2];
	scanf("%s", in);
	int i, j, flag;
	for (i = 0; in[i] != '\0'; ++i)
	{
		flag = 0;
		for (j = 0; s1[j] != '\0' && flag == 0; ++j)
		{
			if (in[i] == s1[j])
			{
				out[i] = s2[j];
				flag = 1;
			}
		}
		if (flag == 0) out[i] = in[i];
	}
	out[i] = '\0';


	printf("\n%s", out);
	system("pause");
	return 0;
}