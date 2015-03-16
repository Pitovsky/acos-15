#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#include <string.h>
#include <stdio.h>

void handler(int signum){
    printf("waken %d\n", signum);
}

int main(int argc, char** argv){
    printf("my pid is: %d\n", getpid());

    int fd = open("buf.bin",
                  O_RDWR | O_CREAT,
                  S_IRWXU
            );

    if(fd == -1){
        perror("can't open");
        _exit(1);
    }

    ftruncate(fd, 20 * 10);

    void * file_addr = mmap(NULL,
                            20 * 10,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,
                            0);

    if (file_addr == NULL){
        perror("map failed");
        return 1;
    }

    if(argc == 3){
        int i;
        int dstpid;
        sscanf(argv[2], "%d", &dstpid);
        for(i = 0; i < 10; ++i){
            fgets(file_addr + 20 * i, 20, stdin);
            kill(dstpid, SIGUSR1);
        }
    }else{
        struct sigaction handler_info;
        memset(&handler_info, 0, sizeof(handler_info));
        handler_info.sa_handler = &handler;
        sigaction(SIGUSR1, &handler_info, NULL);
        while(1){
            pause();
            write(1, file_addr, 20 * 10);
            write(1, "--\n", 3);
        }
    }

    munmap(file_addr, 20 * 10);
    close(fd);
    return 0;
}
