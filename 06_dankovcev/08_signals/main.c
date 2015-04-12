#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

const unsigned DELAY_MS = 1;
int exit_flag = 0;

void sigint_handler(int sig)
{
    printf("Signal cathced.\n");
    exit_flag = 1;
}

void calc_exp(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (exit_flag == 0)
    {
        double sum = exp(start);
        double fact = 1;
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += exp(start) * pow(delta, i) / fact;
            fact *= (i + 1);
        }

        printf("exp(%lf) == %lf (x -> %lf)\n", delta + start, sum, start);
        delta += step;
        sleep(DELAY_MS);
    }

    exit_flag = 0;
}

void calc_ln(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (exit_flag == 0)
    {
        double sum = log(start);
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += pow(-1, i - 1) * pow(delta, i) / (pow(start, i) * i);
        }

        printf("ln(%lf) == %lf (x -> %lf)\n", delta + start, sum, start);
        delta += step;
        sleep(DELAY_MS);
    }

    exit_flag = 0;
}

void calc_sin(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (exit_flag == 0)
    {
        double sum = sin(start);
        double fact = 1;
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += sin(start + M_PI_2 * i) * pow(delta, i) / fact;
            fact *= (i + 1);
        }

        printf("sin(%lf) == %lf (x -> %lf)\n", delta + start, sum, start);
        delta += step;
        sleep(DELAY_MS);
    }

    exit_flag = 0;
}

void calc_cos(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (exit_flag == 0)
    {
        double sum = sin(start);
        double fact = 1;
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += sin(start + M_PI_2 * i) * pow(delta, i) / fact;
            fact *= (i + 1);
        }

        printf("cos(%lf) == %lf (x -> %lf)\n", delta + start, sum, start);
        delta += step;
        sleep(DELAY_MS);
    }

    exit_flag = 0;
}

void run_calc(char* funct, double start, double step, int deep)
{
    if (strcmp(funct, "exp") == 0)
    {
        calc_exp(start, step, deep);
        return;
    }

    if (strcmp(funct, "ln") == 0)
    {
        calc_ln(start, step, deep);
        return;
    }

    if (strcmp(funct, "sin") == 0)
    {
        calc_sin(start, step, deep);
        return;
    }

    if (strcmp(funct, "cos") == 0)
    {
        calc_sin(start, step, deep);
        return;
    }

    fprintf(stderr, "Wrong arguments.\n");
    exit(EXIT_FAILURE);
}

int main(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigint_handler;
    sigaction(SIGINT, &act, 0);

    while(1)
    {
        printf("Enter a command (<func> <start> <step> <deep>):  ");
        int deep;
        double step, start;
        char func[4];
        int read_code = scanf("%s %lf %lf %d", func, &start, &step, &deep);

        if (read_code == EOF)
            break;

        if (read_code != 4)
        {
            fprintf(stderr, "Wrong input.\n");
            return 1;
        }
        run_calc(func, start, step, deep);
    }
    return 0;
}

