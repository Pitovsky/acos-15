#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void mywriter(int fd, char *buf, size_t len){
    while(len > 0){
        ssize_t ret;
        ret = write(fd, buf, len);
        if(ret < 0){
            _exit(errno);
        }

        len -= ret;
        buf += ret;
    }        
}

int main(){
    FILE* f = fopen("tmp.txt", "w");

    fprintf(f, "cde\n");
    fclose(f);

    int d= open("tmp.txt", O_WRONLY | O_APPEND );
    mywriter(d, "xyz\n", 4);
    close(d);

    return 0;
}
