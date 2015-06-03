#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>




sig_atomic_t flag = 0;

void got_sig(int sig)
{
	printf("Caught.\n");
	flag = 1;
};

void Exp(double start, double step, int deep)
{
	assert(deep >= 0);
	double delta = 0;

	while (flag == 0)
	{
		double sum = exp(start);
		double fact = 1;
		int i;
		for (i = 1; i <= deep; ++i)
		{
			sum += exp(start) * pow(delta, i) / fact;
			fact *= (i + 1);
		}
		printf("exp%lf == %lf \n", delta + start, sum);
		delta += step;
	}

	flag = 0;
};

void ln(double start, double step, int deep)
{
	assert(deep >= 0);
	double delta = 0;
	while (flag == 0)
	{
		double sum = log(start);
		int i;
		for (i = 1; i <= deep; ++i)
		{
			sum += pow(-1, i - 1) * pow(delta, i) / (pow(start, i) * i);
		}
		printf("ln%lf == %lf \n", delta + start, sum);
		delta += step;
	}
	flag = 0;
};

void Sin(double start, double step, int deep)
{
	assert(deep >= 0);
	double delta = 0;
	while (flag == 0)
	{
		double sum = sin(start);
		double fact = 1;
		int i;
		for (i = 1; i <= deep; ++i)
		{
			sum += sin(start + M_PI_2 * i) * pow(delta, i) / fact;
			fact *= (i + 1);
		}
		printf("Синус от %lf равен %lf\n", delta + start, sum);
		delta += step;
	}
	flag = 0;
};

void Cos(double start, double step, int deep)
{
	assert(deep >= 0);
	double delta = 0;
	while (flag == 0)
	{
		double sum = cos(start);
		double fact = 1;
		int i;
		for (i = 1; i <= deep; ++i)
		{
			sum += cos(start + M_PI/2 * i) * pow(delta, i) / fact;
			fact *= (i + 1);
		}
		printf("Косинус от %lf равен %lf\n", delta + start, sum);
		delta += step;
	}
	flag = 0;
};

void Calculate(char* funct, double start, double step, int deep)
{
	if (strcmp(funct, "exp") == 0)
	{
		Exp(start, step, deep);
		return;
	}

	if (strcmp(funct, "ln") == 0)
	{
		ln(start, step, deep);
		return;
	}

	if (strcmp(funct, "sin") == 0)
	{
		Sin(start, step, deep);
		return;
	}

	if (strcmp(funct, "cos") == 0)
	{
		Cos(start, step, deep);
		return;
	}

	fprintf(stderr, "Некорректные значения\n");
	exit(EXIT_FAILURE);
};

int main(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = got_sig;
	sigaction(SIGINT, &act, 0);

	while(1)
	{
		printf("Введите команду");
		int deep;
		double step, start;
		char func[4];
		int read = scanf("%s %lf %lf %d", func, &start, &step, &deep);

		if (read == EOF) break;

		if (read != 4)
		{
			fprintf(stderr, "Ошибка ввода\n");
			return(1);
		}
		Calculate(func, start, step, deep);
	}
	return(0);
};


