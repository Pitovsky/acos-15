#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

unsigned degree;
unsigned N;
double* coeffs;
double* dots;
double* answers;
pthread_mutex_t mutex_for_answers;


typedef struct {
    unsigned num;
} JobInfo;

void* calc_func2(void* arg){
    JobInfo* info = arg;

    double coeff = coeffs[info->num];
    unsigned mydegree = info->num;

    unsigned i;
    double answer = 0;
    double curMult = 1;
    for(i = 0; i < N; ++i){
        unsigned j;
        answer = coeff;
        for(j = 0; j < mydegree; ++j){
            answer *= dots[i];
        }
        pthread_mutex_lock(&mutex_for_answers);
        answers[i] += answer;
        pthread_mutex_unlock(&mutex_for_answers);
    }
    
    free(arg);
    return 0;
}

int main(){
    
    scanf("%u", &degree);
    unsigned i = 0;
    scanf("%u", &N);

    coeffs = (double*) malloc((degree + 1) * sizeof(double));
    dots = (double*) malloc( N * sizeof(double));
    answers = (double*) malloc(N * sizeof(double));

    memset(answers, 0, N * sizeof(double));

    pthread_t* jobs = malloc( (degree + 1) * sizeof(pthread_t));

    pthread_mutex_init(&mutex_for_answers, NULL);

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

    for(i = 0; i != degree + 1; ++i){
        JobInfo* jobinfo = malloc(sizeof(jobinfo));
        jobinfo->num = i;
        if(pthread_create(jobs + i, NULL, &calc_func2, jobinfo) != 0){
            perror("can't create thread");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i != degree + 1; ++i){
        pthread_join(jobs[i], NULL);
        printf("value in dot %lf is %lf\n", dots[i], answers[i]);
    }

    free(coeffs);
    free(dots);
    free(answers);

    pthread_mutex_destroy(&mutex_for_answers);

    return EXIT_SUCCESS;
}
