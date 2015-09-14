#include <string.h>
#include <stdio.h>
#include <signal.h>
#include<math.h>
#include<unistd.h>

#define SLEEP_TIME 1
#define EPSIL 0.000001

int flag = 0;
void handler(int signum){
	printf("waken %d\n", signum);
	flag = 1;
}

double my_pow(double x, int deg){
	double res = 1;
	if(deg>0){
		int i;
		for(i = 1; i <= deg; ++i)
			res *= x;
	}
	return res;
}	

int fact(int k){
	int res = 1;
	int i;
	for (i = 1; i<=k; ++i){
		res*=i;
	}
	return res;
}

void my_cos(double start, double step, int deep){
	double res;
	double x = start;
	while(flag == 0){
		res = cos(x-EPSIL);
	
		if (deep>0){
			double f = 1;
			int i;
			for(i = 1; i<= deep; i++){
				res+=cos(x-EPSIL+M_PI_2*i)*pow(EPSIL, i)/f;
				f *= i+1;
			}
		}
		printf("cos(%lf) = %lf\n", x, res);
		x+=step;
		sleep(SLEEP_TIME);
	}
	flag = 0;
}

void my_sin(double start, double step, int deep){
	double res;
	double x = start;
	while(flag == 0){
		res = sin(x-EPSIL);
	
		if (deep>0){
			double f = 1;
			int i;
			for(i = 1; i<= deep; i++){
				res+=sin(x-EPSIL+M_PI_2*i)*pow(EPSIL, i)/f;
				f *= i+1;
			}
		}
		printf("sin(%lf) = %lf\n", x, res);
		x+=step;
		sleep(SLEEP_TIME);
	}
	flag = 0;
}

void my_exp(double start, double step, int deep){
double res;
	double x = start;
	while(flag == 0){
		res = exp(x-EPSIL);
	
		if (deep>0){
			double f = 1;
			int i;
			for(i = 1; i<= deep; i++){
				res+=exp(x-EPSIL)*pow(EPSIL, i)/f;
				f *= i+1;
			}
		}
		printf("exp(%lf) = %lf\n", x, res);
		x+=step;
		sleep(SLEEP_TIME);
	}
	flag = 0;
}

void my_ln(double start, double step, int deep){
double res;
	double x = start;
	while(flag == 0){
		res = log(x-EPSIL);
	
		if (deep>0){
			double f = 1;
			int i;
			for(i = 1; i<= deep; i++){
				res+=pow(x-EPSIL, (-1)*i)*(1-i)*pow(EPSIL-1, i)/f;
				f *= i+1;
			}
		}
		printf("ln(%lf) = %lf\n", x, res);
		x+=step;
		sleep(SLEEP_TIME);
	}
	flag = 0;
}

int main(){

	struct sigaction handler_info;
    memset(&handler_info, 0, sizeof(handler_info));
    handler_info.sa_handler = &handler;
    sigaction(SIGINT, &handler_info, NULL);

	char func[3];
	double start, step;
	int deep;
	while(1){
	if(scanf("%s %lf %lf %d", func, &start, &step, &deep) == EOF)
		break;
	//проверка правильности ввода
	//if(deep<0)


	if(strcmp(func, "cos") == 0) my_cos(start, step, deep);
	else if(strcmp(func, "exp") == 0) my_exp(start, step, deep);
	else if(strcmp(func, "sin") == 0) my_sin(start, step, deep);
	else if(strcmp(func, "ln") == 0) my_ln(start, step, deep);
	else if(strcmp(func, "exit") == 0) break;

	}



	return 0;
}
