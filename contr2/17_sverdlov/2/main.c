#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


char *file_name;
char *file;
//это вообще что?
int count[256];

int main(int argc, char **argv)
{
	int socket_ofclient, listener;
        struct sockaddr_in addr;

        char buf[1024];
        int readres;
        listener = socket(AF_INET, SOCK_STREAM, 0);

        if(listener < 0) {
            perror("failded to create socket");
            return (1);
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(1111);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(listener, (struct sockaddr*) &addr, sizeof(addr)) < 0){
            perror("can't bind");
            return (1);
        }

        listen(listener, 1);

        socket_ofclient = accept(listener, NULL, NULL);
        if(socket_ofclient < 0){
            perror("can't make connection");
            return (1);
        }

        int readed = read(socket_ofclient, buf, 1024);
        write(socket_ofclient, "got:\n", 5);
        write(socket_ofclient, buf, readed);

	return 0;
}
