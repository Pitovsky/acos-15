#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Wrong args.\n");		
	}
	unsigned ip1 = inet_addr(argv[1]);
	unsigned ip2 = inet_addr(argv[2]);
	unsigned mask = inet_addr(argv[3]);
	//printf("%u %u %u\n", ip1, ip2, mask);
	if ((ip1 & mask) == (ip2 & mask))
	{
		printf("1\n");
	}
	else
	{
		printf("0\n");
	}
	return EXIT_SUCCESS;
}