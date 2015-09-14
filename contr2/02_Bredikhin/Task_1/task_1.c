#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>

int main(int argc, char** argv){
	in_addr_t First = inet_addr(argv[1]);
	in_addr_t Second = inet_addr(argv[2]);
	in_addr_t Third = inet_addr(argv[3]);
	if ( (First & Third) == (Second & Third) )
	{
		printf("1\n");
	}
	else
	{
		printf("0\n");
	}
	return 0;
}
