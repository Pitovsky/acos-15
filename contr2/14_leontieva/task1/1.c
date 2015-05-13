#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char** argv){
if (argc < 4) {
	perror("too few arguments");
	return 0;;
}

if ((inet_addr(argv[1]) & inet_addr(argv[3])) == (inet_addr(argv[2]) & inet_addr(argv[3]))){
	printf("1\n");
	return 1;
}
else{
 printf ("0\n");
 return 0;
}
}
