#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
void handler(int signum){
    printf("waken %d\n", signum);
}

int main(int argc, char** argv){
    printf("my pid is: %d\n", getpid());

    int fd = open("buf.bin",
                  O_RDWR | O_CREAT,
                  S_IRWXU
            );

    sem_t* semdescr = sem_open("sem_for_data", O_CREAT | O_RDWR, S_IRWXU, 1); 

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 10;
    attr.mq_curmsgs = 0;
    mqd_t que = mq_open("queue4", O_CREAT | O_RDWR, S_IRWXU, &attr);

    if(fd == -1 || (que == (mqd_t)-1 )){
        perror("can't open");
        _exit(errno);
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
            sem_wait(semdescr);
            fgets(file_addr + 20 * i, 20, stdin);
            sem_post(semdescr);
            if( mq_send(que,(char*) &i, 4,(unsigned) 0) < 0){
                 perror("can't send");   
                 return 1;
            }
        }
    }else{
        while(1){
            int rowofchange;
            mq_receive(que,(char*) &rowofchange, 4, NULL);
            sem_wait(semdescr);
            printf("change in row %d detected\n", rowofchange);
            write(1, file_addr, 20 * 10);
            sem_post(semdescr);
            write(1, "--\n", 3);
        }
    }

    munmap(file_addr, 20 * 10);
    close(fd);
    return 0;
}
