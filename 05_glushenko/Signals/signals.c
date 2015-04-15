#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>


sig_atomic_t flag = 0;

void sigint_handler(int sig)
{
    printf("Caught.\n");
    flag = 1;
}

void Eexp(double start, double step, int deep)
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
}

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
}

void Ssin(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (flag == 0)
    {
        double sum = sin(start);
        double factr = 1;
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += sin(start + M_PI_2 * i) * pow(delta, i) / factr;
            factr *= (i + 1);
        }

        printf("sin%lf == %lf\n", delta + start, sum);
        delta += step;
    }

    flag = 0;
}

void Ccos(double start, double step, int deep)
{
    assert(deep >= 0);
    double delta = 0;

    while (flag == 0)
    {
        double sum = cos(start);
        double factr = 1;
        int i;

        for (i = 1; i <= deep; ++i)
        {
            sum += cos(start + M_PI/2 * i) * pow(delta, i) / factr;
            factr *= (i + 1);
        }

        printf("cos(%lf) == %lf\n", delta + start, sum);
        delta += step;
    }

    flag = 0;
}

void Calculate(char* funct, double start, double step, int deep)
{
    if (strcmp(funct, "exp") == 0)
    {
        Eexp(start, step, deep);
        return;
    }

    if (strcmp(funct, "ln") == 0)
    {
        ln(start, step, deep);
        return;
    }

    if (strcmp(funct, "sin") == 0)
    {
        Ssin(start, step, deep);
        return;
    }

    if (strcmp(funct, "cos") == 0)
    {
        Ccos(start, step, deep);
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
        int read = scanf("%s %lf %lf %d", func, &start, &step, &deep);

        if (read == EOF)
            break;

        if (read != 4)
        {
            fprintf(stderr, "Wrong input.\n");
            return 1;
        }
        Calculate(func, start, step, deep);
    }
    return 0;
}
