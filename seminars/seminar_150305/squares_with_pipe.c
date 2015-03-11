#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int child_process(int read_fd, int write_fd, int id){
   int task = 0;
   int sum = 0;
   while(4 == read(read_fd, &task, sizeof(task))){
        printf("task %d is taken by child %d\n", task, id);
        sum += task * task;
   }
   write(write_fd, &sum, sizeof(sum));
   return 0;
}

int main(){
    
    // создание pipe для обмена информацией
    int task_pipes[2];
    int answers_pipes[2];

    int code;
    code = pipe(task_pipes);
    code = (code || pipe(answers_pipes));

    if(!code){
       // perror("can't open pipes");
       // return code;
    }
    //forks
    
    pid_t pid = fork();
    if(pid == 0){
        close(task_pipes[1]);
        return child_process(task_pipes[0], answers_pipes[1], 1);
    }    

    pid = fork();
    if(pid == 0){
        close(task_pipes[1]);
        return child_process(task_pipes[0], answers_pipes[1], 2);
    } 
    //раздача заданий
    int task;
    while(1 == scanf("%d", &task)){
        ssize_t a;
        a = write(task_pipes[1], &task, sizeof(task));
        if(a != sizeof(task)){
            perror("can't write to pipe");
            return 1;
        }
    }

    close(task_pipes[1]);
    
    //агрегация
    int sum = 0;
    int tmp;
    read(answers_pipes[0], &tmp, sizeof(tmp));
    sum += tmp;
    read(answers_pipes[0], &tmp, sizeof(tmp));
    sum += tmp;

    printf("result is: %d\n", sum);


    return 0;
}
