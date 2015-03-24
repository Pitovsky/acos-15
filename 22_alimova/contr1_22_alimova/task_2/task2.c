#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	char *s1, *s2;
	char str[20];
	s1 = argv[1];
	s2 = argv[2];
	gets(str);
	printf("\n");
	int i,j,k;
	for(i=0;i<strlen(str);i++)
		{

				for(j=0;j<strlen(s1);j++)
				{
					if(str[i]==s1[j])
						{
							str[i]=s2[j];
							break;
						}
				}
	}

	printf("%s", str);
	printf("\n");
}