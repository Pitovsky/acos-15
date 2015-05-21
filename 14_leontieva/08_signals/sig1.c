#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

unsigned deep;
char func[4];
int flag = 0;

double fact(double a){
	int res=1, i;
	for(i = 2; i <= a;i ++)
		res *= i;
	return res;
}
double ln(double *d){
    unsigned i;
    double answer = 1;
    double curMult = *d;
    for(i = 1; i < deep + 1;i++){
        answer += curMult/i;
        curMult *= *d;
    }
    return answer;
}

double exp(double *d){
    unsigned i;
    double answer = 0;
    double curMult = *d;
    for(i = 1; i < deep + 1;i++){
        answer += curMult/fact(i);
        curMult *= *d;
    }
    return answer;
}

double sin(double *d){
    unsigned i;
    double answer = 0;
    double curMult =* d;
    for(i = 1; i <= (deep+1)/2; i++){
        if ( i % 2 == 0) answer -= curMult/fact(2*i -1);
        else answer += curMult/fact(2*i -1);
        curMult *= (*d)*(*d);
    }
    return answer;
}

double cos(double *d){
    unsigned i;
    double answer = 0;
    double curMult = 1;
    for(i = 0; i <= deep/2; i++){
        if ( i % 2 == 0) answer += curMult/fact(2*i);
        else answer -= curMult/fact(2*i);
        curMult *= (*d)*(*d);
    }
    return answer;
}


void calc(double start, double step){
		if (strcmp("exp", func) == 0){
			while (flag){
				printf("exp in dot %lf is %lf\n", start,exp(&start));
				start += step;
            }
        }
        if (strcmp("sin", func) == 0){
            while (flag){
				printf("sin in dot %lf is %lf\n", start,sin(&start));
				start += step;
            }
        }
        if (strcmp("cos", func) == 0){
            while (flag){
				printf("cos in dot %lf is %lf\n", start,cos(&start));
				start += step;
            }
        }
        if (strcmp("ln", func) == 0){
            while(flag){
                printf("ln in dot %lf is %lf\n", start, ln(&start));
                start += step;
            }
        }
}

void do_calc(){
        double start;
        double step;
        flag = 1;
        memset(&func, 0, 4*sizeof(char));
        scanf("%s", &func);
        scanf("%d", &deep);
        scanf("%lf", &start);
        scanf("%lf", &step);
        calc(start, step);

}
static void handler(int signum){
	flag = 0;
	//do_calc();
}

int main(){
    struct sigaction act;
	memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigaction(SIGINT, &act, NULL);
    while(flag == 0) do_calc();
	return 0;
}
