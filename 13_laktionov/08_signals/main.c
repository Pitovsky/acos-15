#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#define pi 3.14159265359
struct sigaction act;
int flag = 1;

void countExp(char* func, double start, double step, int deep) {
    
    double delta = step;
    double sum = 0;
    int k = 1;
    double add;
    
    while(flag) {
        if (strcmp(func, "exp")==0)
            sum = exp(start);
        if (strcmp(func, "ln")==0)
            sum = log(start);
        if (strcmp(func, "sin")==0)
            sum = sin(start);
        if (strcmp(func, "cos")==0)
            sum = cos(start);
        for (int i = 1; i <= deep; ++i){
            add = (exp(start)/k)*pow(delta, i);
            sum += add;
            k*=(k+1);
        }
        printf("sum: %lf\n", sum);
        sum = exp(start);
        k = 1;
        delta += step;
        sleep(1);
    }
    flag = 1;
}

//void countLn(double start, double step, double deep) {
//    
//    double delta = step;
//    double sum = log(start);
//    double add;
//
//    while(flag) {
//        for (int i = 1; i <= deep; ++i) {
//            add = 1/(pow(start, i)*i)*pow(delta, i);
//            if (i%2==0)
//                add *= (-1);
//            sum += add;
//        }
//        printf("sum: %lf\n", sum);
//        sum = log(start);
//        delta += step;
//        sleep(1);
//    }
//    flag = 1;
//}
//
//void countSin(double start, double step, double deep) {
//    
//    double delta = step;
//    double sum = sin(start);
//    int k = 1;
//    double add;
//    
//    while(flag) {
//        for (int i = 1; i <= deep; ++i) {
//            if (i%2 == 0)
//                add = (sin(start)/k)*pow(delta, i);
//            else
//                add = (cos(start)/k)*pow(delta, i);
//            if (i%4 == 2 || i%4 == 3)
//                add *= (-1);
//            sum += add;
//            k*=(k+1);
//        }
//        printf("sum: %lf\n", sum);
//        sum = sin(start);
//        k = 1;
//        delta += step;
//        sleep(1);
//    }
//    flag = 1;
//}

void input() {
    double start, step, deep;
    char* func = (char*)malloc(sizeof(char)*5);
    printf("\ncommand: ");
    scanf("%s %lf %lf %lf", func, &start, &step, &deep);
    countExp(func, start, step, deep);
//    if (strcmp(func, "exp")==0) {
//        countExp(start, step, deep);
//    }else if (strcmp(func, "ln")==0) {
//        countLn(start, step, deep);
//    }else if (strcmp(func, "sin")==0) {
//        countSin(start, step, deep);
//    }else if (strcmp(func, "cos")==0) {
//        countSin(start + pi/2, step, deep);
//    }else{
//        printf("wrong command");
//        input();
//    }
}

static void input_handler(int signum) {
    flag = 0;
    input();
}

int main() {
    
    memset(&act, 0, sizeof(act));
    act.sa_handler = input_handler;
    sigaction(SIGINT, &act, NULL);
    
    input();

    return 0;
}
