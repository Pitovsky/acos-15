#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int main(int argc, char** argv){
	int ip1, ip2, mask;
	struct sockaddr_in firstAddress;
	struct sockaddr_in secondAddress;
	struct sockaddr_in maska;
	if(argc != 4){
		printf("wrong number of args\n");
		return -1;
	}
	if(inet_aton(argv[1], &firstAddress.sin_addr) == 0 || inet_aton(argv[2], &secondAddress.sin_addr) == 0 || inet_aton(argv[3], &maska.sin_addr) == 0){
		printf("wrong input");
		return -1;
	}
	ip1 = firstAddress.sin_addr.s_addr;
	ip2 = secondAddress.sin_addr.s_addr;
	mask = maska.sin_addr.s_addr;
	if((ip1 & mask) == (ip2 & mask)){
		printf("1\n");
	}
	else {
		printf("0\n");
	}
	return 0;
}
