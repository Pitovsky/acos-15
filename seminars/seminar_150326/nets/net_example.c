#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
int main(int argc, char** argv){
    if(argc == 1){//server case
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
    }else{
        int sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
            perror("can't create sock");
            return (1);
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(1111);
        addr.sin_addr.s_addr = inet_aton(argv[1]);//htonl();
        //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        
        if(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0){
            perror("can't connect");
            return (1);
        }

        write(sock, "abc\n",4);
        char buf[1024];
        int got = read(sock, buf, 1024);

        write(1, buf, got);
    }
}
