#include<unistd.h>
#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdlib.h>

int f(char* s1, char* s2, char* s3){
	in_addr_t ip1 = inet_addr(s1);
	in_addr_t ip2 = inet_addr(s2);
	in_addr_t ip3 = inet_addr(s3);
	return((ip1&ip3) == (ip2&ip3));
}


int main(){
	char* s1 = (char*)malloc(sizeof(char)*16);
	char* s2 = (char*)malloc(sizeof(char)*16);
	char* s3 = (char*)malloc(sizeof(char)*16);
	scanf("%s", s1);
	scanf("%s", s2);
	scanf("%s", s3);
	//in_addr_t k = inet_addr(s1);
	printf("%d\n", f(s1, s2, s3));

	return 0;
}
