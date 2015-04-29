#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>

int main(int argc, char** argv)
{
    if(argc == 1) {
        int sock, listener;
        struct sockaddr_in addr;
        char buf[1024];
        int bytes_read;
        
        listener = socket(AF_INET, SOCK_STREAM, 0);
        if(listener < 0)
        {
            perror("socket");
            return(1);
        }
        
        addr.sin_family = AF_INET;
        addr.sin_port = htons(3425);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("bind");
            return(2);
        }
        
        
        listen(listener, 2);
        
        int fds_inuse[100];
        int i;
        for(i = 0; i < 100; ++i) {
            fds_inuse[i] = 0;
        }
        
        while(1)
        {
            fd_set foread;
            FD_ZERO(&foread);
            FD_SET(listener, &foread);
            int max = listener;
            for(i = 0; i < 100; ++i){
                if(fds_inuse[i] != 0){
                    FD_SET(fds_inuse[i], &foread);
                }
                if(max < fds_inuse[i]) max = fds_inuse[i];
            }
            
            int code = select(max+1, &foread, NULL, NULL, NULL);
            
            if (code < 0){
                perror("select erro\n");
            }
            
            if(FD_ISSET(listener, &foread)){
                sock = accept(listener, NULL, NULL);
                if(sock < 0)
                {
                    perror("accept");
                    return(3);
                }
                
                for(i = 0; i < 100; ++i){
                    if(fds_inuse[i] == 0){
                        fds_inuse[i] = sock;
                        i = -1;
                        break;
                    }
                }
                if(i > 0){
                    perror("too many connections");
                    return 1;
                }
            }
            
            for(i = 0; i < 100; ++i){
                if(fds_inuse[i] != 0 &&
                   FD_ISSET(fds_inuse[i], &foread)
                   )
                {
                    bytes_read = recv(fds_inuse[i], buf, 1024, 0);
                    if(bytes_read <= 0) {
                        close(fds_inuse[i]);
                        fds_inuse[i] = 0;
                        continue;
                    }
                    printf("server got: %s", buf);
                    send(sock, buf, bytes_read, 0);
                }
            }
        }
    }else{
        int sock;
        struct sockaddr_in addr;
        
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0)
        {
            perror("socket");
            return(1);
        }
        
        addr.sin_family = AF_INET;
        addr.sin_port = htons(3425);
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            return(2);
        }
        
        char buf[1024];
        int i;
        for(i = 0; i < 10; ++i){
            fgets(buf, 1000,stdin);
            send(sock, buf, strlen(buf) + 1, 0);
            recv(sock, buf, 1024, 0);
        }
        
        printf("%s", buf);
        close(sock);
        
        return 0;
    }
}