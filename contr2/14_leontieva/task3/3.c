#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
if (argc < 4) {
	perror("too few arguments");
	return -1;
}
struct sockaddr_in	server_addr;
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons((unsigned short)argv[2]);
server_addr.sin_addr.s_addr = inet_addr(argv[1]);
int sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0) {
	perror("can't create socket");
	return -1;
}
if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
	perror("can't connect with the server");
	return -1;
}

FILE* f = fopen(argv[3], "r");
if (f == NULL) {
	perror("can't open file");
	return -1;
}
int num_s = 0;
char buf [512], tmp[512];
do{
	fgets(buf, 512, f);
	printf("%s", buf);
	if (num_s % 2 == 0) {
		strcpy(tmp, buf);
	}
	if (num_s % 2 == 1) {
		send(sock, buf, sizeof(buf),0)//а если с первого раза не получится всё отправить
		if (recv(sock, buf, sizeof(buf),0) < 0){
			perror("can't recieve msg");
			return -1;
		}
		if (strcmp(tmp, buf) == 0) {
			printf("%d\n",num_s/2);
		}
	}
	num_s++;
}while (buf != EOF);//??? без комментариев просто

return 0;
}

		

return 0;
}
