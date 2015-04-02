#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned degree;
double* coeffs;
double* dots;
double* answers;

typedef struct {
    unsigned num;
} JobInfo;

void* calc_func(void* arg){
    JobInfo* info = arg;

    double dot = dots[info->num];

    unsigned i;
    double answer = 0;
    double curMult = 1;
    for(i = 0; i < degree + 1; ++i){
        answer += curMult * coeffs[i];
        curMult *= dot;
    }
    answers[info->num] = answer;
    
    free(arg);
    return 0;
}

int main(){
    
    scanf("%u", &degree);
    unsigned N;
    unsigned i = 0;
    scanf("%u", &N);

    coeffs = (double*) malloc((degree + 1) * sizeof(double));
    dots = (double*) malloc( N * sizeof(double));
    answers = (double*) malloc(N * sizeof(double));

    pthread_t* jobs = malloc(N * sizeof(pthread_t));

    if( coeffs == 0 || dots == 0 ||  answers == 0 || jobs == 0){
        perror("can't allocate");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < degree + 1; ++i){
        scanf("%lf", coeffs + i);
    }

    for(i = 0; i < N; ++i){
        scanf("%lf", dots + i);
    }

    for(i = 0; i != N; ++i){
        JobInfo* jobinfo = malloc(sizeof(jobinfo));
        jobinfo->num = i;
        if(pthread_create(jobs + i, NULL, &calc_func, jobinfo) != 0){
            perror("can't create thread");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i != N; ++i){
        pthread_join(jobs[i], NULL);
        printf("value in dot %lf is %lf\n", dots[i], answers[i]);
    }

    free(coeffs);
    free(dots);
    free(answers);

    return EXIT_SUCCESS;
}
