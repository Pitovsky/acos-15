#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    if(argc == 2){
        unsigned x;
        if(sscanf(argv[1], "%u", &x) == 1){
            int retval = fork();
            int cmp, i;
            if(retval == 0){//child
                for(i = 0; i < x; ++i){
                    printf("%d: \t %d\n", i, i*i);
                }
            }else{//parent
                wait();                        
                printf("last: %d \n", x * x);
            }

       }
    }
    return 0;
}
