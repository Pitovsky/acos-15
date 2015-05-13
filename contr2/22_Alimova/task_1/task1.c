#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
	in_addr_t x =  inet_addr(argv[1]);
	in_addr_t y =  inet_addr(argv[2]); 
	in_addr_t z =  inet_addr(argv[3]);
	if ((x&z) == (y&z))
		printf ("%d",1);//немного извращения в студию
	else printf ("%d",0);
}

