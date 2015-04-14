#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <signal.h>

int key = 0;

void sig()
{
    key = 1;
    printf("\nSIGINT detected\n");
}

double fact(int a)
{
    double result = 1;
    if (a == 0)
    {
        result = 1;
    }
    for (int i=2; i <= a; i++)
    {
        result *= i;
    }
    return result;
}

/*void processing(char* func, double start, double step, int deep)
{
    double result, point;
    point = start;
    while (key == 1)
    {
        result = 0;
        for (int i=0; i<= deep; i++)
*/

int main()
{
    char func[5];
    int deep, read_key;
    double start, step, result, point, ex;
    signal(SIGINT, sig);
    printf("command input format: *function* *start* *step* *deep*\navailable function: cos, sin, ln, exp\nif you want to exit write exit\n");
    while(1)
    {
        scanf("%s", func);
        if (strcmp(func, "exit") == 0)
        {
            return 0;
        }
        if ((strcmp(func, "cos") != 0) && (strcmp(func, "sin") != 0) && (strcmp(func, "ln") != 0) && (strcmp(func, "exp") != 0))
        {

            printf("command not found\n");
            continue;
        }
        read_key = scanf("%lf %lf %d", &start, &step, &deep);
        if (read_key != 3)
        {
            printf("incorrect format\n");
            continue;
        }
        point = start;
        if (strcmp(func, "cos") == 0)
        {
            while (key == 0)
            {
                result = 0;
                for (int i=0;i<=deep;i++)
                {
                    //printf("%lf * %lf / %d\n", cos(start + i * M_PI_2), pow(point - start, i), fact(i));
                    result += cos(start + i * M_PI_2) * pow(point - start, i) / fact(i);
                }
                printf("%s(%lf) = %lf\n", func, point, result);
                point += step;
                sleep(1);
            }
        }
        if (strcmp(func, "sin") == 0)
        {
            while (key == 0)
            {
                result = 0;
                for (int i=0;i<=deep;i++)
                {
                    result += sin(start + i * M_PI_2) * pow(point - start, i) / fact(i);
                }
                printf("%s(%lf) = %lf\n", func, point, result);
                point += step;
                sleep(1);
            }
        }
        if (strcmp(func, "exp") == 0)
        {
            while (key == 0)
            {
                result = 0;
                ex = exp(start);
                for (int i=0;i<=deep;i++)
                {
                    result += ex * pow(point - start, i) / fact(i);
                }
                printf("%s(%lf) = %lf\n", func, point, result);
                point += step;
                sleep(1);
            }
        }
        if (strcmp(func, "ln") == 0)
        {
            while (key == 0)
            {
                result = 0;
                result += log(start);
                for (int i=1;i<=deep;i++)
                {
                    //printf("%lf * %d * %lf / (%lf * %lf)\n", pow(-1, i-1), (i-1), pow(point - start, i), fact(i), pow(start, i)); 
                    result += pow(-1, i - 1) * pow(point - start, i) / (pow(start, i) * i);
                }
                printf("%s(%lf) = %lf\n", func, point, result);
                point += step;
                sleep(1);
            }
        }
        key = 0;
    }
}
