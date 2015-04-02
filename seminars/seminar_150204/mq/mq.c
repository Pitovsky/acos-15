#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

int main(int argc, char** argv){
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 8;
    attr.mq_msgsize = 10;
    attr.mq_curmsgs = 0;


    mqd_t mqiden = mq_open("/mqt4", O_RDWR | O_CREAT, 0700, &attr);
    if (mqiden < 0) {
        perror("can't open");
        return 1;
    } 

    if (argc == 1){
        int i = 0;
        for( i = 0; i < 10; ++i){
            char buf[1024];
            if(mq_receive(mqiden, buf, 20, NULL) < 0){
                 perror("some_error");
                 return 1;
            }
            printf("%s", buf);
        }
    } else {
        int i = 0;
        for( i = 0; i < 4; ++i){
            char buf[1024];
            fgets(buf, 6, stdin);
            mq_send(mqiden, buf, 7, 0);
        }
    } 

    mq_close(mqiden);
    return 0;
}

