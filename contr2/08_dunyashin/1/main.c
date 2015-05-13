#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
	int ip1;
	int ip2;
	int mask;
	struct sockaddr_in str1;
	struct sockaddr_in str2;
	struct sockaddr_in str3;
	if(inet_aton(argv[1], &str1.sin_addr) == 0 || inet_aton(argv[2], &str2.sin_addr) == 0 || inet_aton(argv[3], &str3.sin_addr) == 0){
		printf("NEVERNIY VVOD");
		return -1;
	}
	ip1 = str1.sin_addr.s_addr;
	ip2 = str2.sin_addr.s_addr;
	mask = str3.sin_addr.s_addr;
	if((ip1 & mask) == (ip2 & mask)){
		printf("1\n");
	}
	else {
		printf("0\n");
	}
	return 0;
}
