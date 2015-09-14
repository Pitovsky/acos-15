
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>

int main(int argc, char** argv){
in_addr_t a = inet_addr(argv[1]);
in_addr_t b = inet_addr(argv[2]);
in_addr_t c = inet_addr(argv[3]);
if ( (a&c) == (b&c) ){printf("1\n");}
else{printf("0\n");}
return 0;
}

