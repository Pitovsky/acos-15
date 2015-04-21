#include <math.h>
#include <signal.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int is_processing;

void compute_and_print(double start, double step, double* koefs, 
					   int deep, char* func_name)
{
	printf("in computing...\n");
	int i, deg;
	for (i = 0; is_processing; ++i) {
		double ans = 0.0;
		double x = start + i * step;
		double x_mult = 1;
		for (deg = 0; deg <= deep; ++deg) {
			ans += koefs[deg] * x_mult;
			x_mult *= (x - start);
		}
		printf("Calculated by Taylor in %lf: \
				%s(%lf) ~= %lf\n", start, func_name, x, ans);
	}
}

void get_cos_koefs(double start, int deep, double *koefs)
{
	int i;
	double fact = 1;
	for (i = 0; i <= deep; ++i) {
		switch (i & 3) { // i modulo 4
			case 0:
				koefs[i] = cos(start);
				continue;
			case 1:
				koefs[i] = -sin(start);
				continue;
			case 2:
				koefs[i] = -cos(start);
				continue;
			case 3:
				koefs[i] = sin(start);
		}
		koefs[i] /= fact;
		fact *= i + 1;
	}
}

void get_sin_koefs(double start, int deep, double *koefs)
{
	int i;
	double fact = 1;
	for (i = 0; i <= deep; ++i) {
		switch (i & 3) { // i modulo 4
			case 0:
				koefs[i] = sin(start);
				continue;
			case 1:
				koefs[i] = cos(start);
				continue;
			case 2:
				koefs[i] = -sin(start);
				continue;
			case 3:
				koefs[i] = -cos(start);
		}
		koefs[i] /= fact;
		fact *= i + 1;
	}
}

void get_exp_koefs(double start, int deep, double *koefs)
{
	int i;
	double fact = 1;
	koefs[0] = exp(start);
	for (i = 1; i <= deep; ++i) {
		koefs[i] = koefs[i - 1] / i;
	}
}

void get_ln_koefs(double start, int deep, double *koefs)
{
	int i;
	double fact = 1;
	koefs[0] = log(start);
	if (deep) {
		koefs[1] = 1 / start;
	}
	for (i = 2; i <= deep; ++i) {
		koefs[i] = -i * koefs[i - 1] / (fact * start);
	}
}

void parse_and_solve(int argc, char **argv)
{
	char buf[7];
	int start, step, deep;
	if (scanf("%s", buf) != 1) {
		printf("Err: <func> must be: cos, sin, exp, \
				ln, exit (for exit)\n");
		return;
	} else if (strcmp(buf, "exit") == 0) {
		printf("exiting..\n");
		exit(0);
	}
	if (scanf("%d", &start) != 1) {
		printf("Err: <start> must be integer\n");
		return;
	}
	if (scanf("%d", &step) != 1) {
		printf("Err: <step> must be integer\n");
		return;
	}
	if (scanf("%d", &deep) != 1) {
		printf("Err: <deep> must be integer\n");
		return;
	}
	
	is_processing = 1;

	double *koefs = malloc(sizeof(double) * (deep + 1));
	if (strcmp(buf, "cos") == 0) {
		get_cos_koefs(start, deep, koefs);
	} else if (strcmp(buf, "sin") == 0) {
		get_sin_koefs(start, deep, koefs);
	} else if (strcmp(buf, "exp") == 0) {
		get_exp_koefs(start, deep, koefs);
	} else if (strcmp(buf, "ln") == 0) {
		get_ln_koefs(start, deep, koefs);
	}
	
	compute_and_print(start, step, koefs, deep, buf);
}

void handler(int sig)
{
	printf("Signal received, exit computations..\n");
	is_processing = 0;
}

int main(int argc, char **argv)
{
	struct sigaction handler_info;
	memset(&handler_info, 0, sizeof(handler_info));
	handler_info.sa_handler = &handler;

	sigaction(SIGINT, &handler_info, NULL);

	while (1) {
		is_processing = 0;
		parse_and_solve(argc, argv);
	}

	return 0;
}
