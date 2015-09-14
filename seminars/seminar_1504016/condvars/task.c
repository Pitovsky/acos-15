#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t m_start;
pthread_cond_t start_info_updater;

int task_type;//1 - calculate, -1 - exit 0 - no new task
unsigned int task_value;

pthread_mutex_t m_result;
pthread_cond_t result_info_updater;

unsigned long long result;
int task_completet;

unsigned long long calc_factorial(unsigned what){
    unsigned long long result = 1;
    while(what > 0){
        result *= what;
        --what;    
    }
    return result;
}

void* worker_func(void* arg){
    int return_code;
    while(1){
        pthread_mutex_lock(&m_start);
        do{//cycle of wait for new task
           if(task_type != 0){//already got new task
              break;
           }     
           printf("waiting for new task\n");
           //next operation automatically unlock mutex on call and lock it on return
           return_code = pthread_cond_wait(&start_info_updater, &m_start);
           if(return_code != 0){
                perror("cond var error");
                exit(EXIT_FAILURE);
           }
        }while(1);

        if(task_type == -1){//command for exit
            pthread_mutex_unlock(&m_start);
            break;
        }

        if(task_type == 1){//command for calculate
            unsigned local_task = task_value;
            task_type = 0;
            pthread_mutex_unlock(&m_start);//all data about the task is already loaded

            //calculation
            unsigned long long res = calc_factorial(local_task);

            //storing result
            pthread_mutex_lock(&m_result);
                task_completet = 1;
                result = res;
                pthread_cond_signal(&result_info_updater);
            pthread_mutex_unlock(&m_result);
        }
    }

    return 0;
}

int main(){
    pthread_mutex_init(&m_start, 0);
    pthread_mutex_init(&m_result, 0);
    pthread_cond_init(&start_info_updater, 0);
    pthread_cond_init(&result_info_updater, 0);
    
    task_completet = 0;
    task_type = 0;

    int return_code;
    pthread_t worker; 

    return_code = pthread_create(&worker, 0, worker_func, 0);

    if(return_code != 0){
        perror("failed to create worker");
        exit(EXIT_FAILURE);
    }
    
    unsigned int task_var;
    while(scanf("%ud", &task_var) == 1){
        pthread_mutex_lock(&m_start);//store result of the task
            task_type = 1;
            task_value = task_var;
            pthread_cond_signal(&start_info_updater); 
        pthread_mutex_unlock(&m_start);

        printf("waiting for computation\n");

        pthread_mutex_lock(&m_result);
        do{//cycle of wait
           if(task_completet == 1){
              break;
           }     
           printf("waiting for computation in cycle\n");
       
           //next operation automatically unlock mutex on call and lock it on return
           return_code = pthread_cond_wait(&result_info_updater, &m_result);
           if(return_code != 0){
                perror("cond var error");
                exit(EXIT_FAILURE);
           }
        } while(1);

        task_completet = 0;
        
        unsigned long long res = result;
        pthread_mutex_unlock(&m_result);

        printf("result of last task is: %llu  \n", res);
    }

    //say to the worker that it's time to die
    pthread_mutex_lock(&m_start);
        task_type = -1;
        pthread_cond_signal(&start_info_updater); 
    pthread_mutex_unlock(&m_start);

    pthread_join(worker, 0);

    return EXIT_SUCCESS;
}
