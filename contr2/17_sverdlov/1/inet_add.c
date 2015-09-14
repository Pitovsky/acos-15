#include <stdio.h>

char buf[256];
unsigned getnum(char* s)
{
	int i;//ну хотя бы sscanf бы вызвали
	unsigned ans = 0;
	int j = 0;
	int len = strlen(s);
	for (i = 0; i < 4; ++i) {
		unsigned x = 0;
		while (j < len && isdigit(s[j]))
			x *= 10, x += (s[j] - '0'), ++j;
		++j;
		ans <<= 8;
		ans |= x;
	}
	return ans;
}

int main(int argc, char ** argv)
{
	unsigned addr1 = getnum(argv[1]);
	unsigned addr2 = getnum(argv[2]);
	unsigned submask = getnum(argv[3]);

	printf("%u %u %u\n", addr1, addr2, submask);

	int good = (addr1 & submask) == (addr2 & submask);
	printf("%d", good);
	return 0;
}
