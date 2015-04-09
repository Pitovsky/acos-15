#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <math.h>

int mione(int n)
{
    if (n % 2 == 0)
        return 1;
    else
        return -1;
}

int lastSig = 0;
void sigcc()
{
    lastSig = 1;
}
void sigus()
{
    lastSig = 2;
}
int main(int argc, char** argv)
{
    char func[3];
    double* coeffs;
    double start = 0;
    double step = 0;
    int deep = 0;
    int i = 0;

    signal(SIGINT, sigcc);

    while (1)
    {
        scanf("%s", func);
        if (strcmp(func, "end") == 0)
            break;
        scanf("%lf", &start);
        scanf("%lf", &step);
        scanf("%d", &deep);
        ++deep;

        coeffs = (double*)malloc(deep*sizeof(double));
        int nowFactorial = 1;
        double nowDiff = 0;
        for (i = 0; i < deep; ++i)
        {
            if (i > 1)
                nowFactorial *= i;
            if (strcmp(func, "sin") == 0)
            {
                if (i % 2 == 0)
                    nowDiff = mione(i/2)*sin(start);
                else
                    nowDiff = mione(i/2)*cos(start);
            }
            else if (strcmp(func, "cos") == 0)
            {
                if (i % 2 == 0)
                    nowDiff = mione(i/2 + 1)*cos(start);
                else
                    nowDiff = mione(i/2 + 1)*sin(start);
            }
            else if (strcmp(func, "exp") == 0)
            {
                nowDiff = exp(start);
            }
            else if (strcmp(func, "ln") == 0 || strcmp(func, "log") == 0)
            {
                nowDiff = 300;
            }
            coeffs[i] = nowDiff/nowFactorial;
        }

        printf("function:\n");
        if (coeffs[0] != 0)
            printf("%lf + ", coeffs[0]);
        for (i = 1; i < deep; ++i)
            if (coeffs[i] != 0)
                printf("%lf*(x - %lf)^%d + ", coeffs[i], start, i);
        printf("o((x - %lf)^%d)\n", start, deep - 1);

        double nowPos = start;

        while (1)
        {
            double res = 0;
            double nowLvl = 1;
            for (i = 0; i < deep; ++i)
            {
                res = res + coeffs[i]*nowLvl;
                nowLvl *= (nowPos - start);
            }
            printf("calculated %s(%lf) = %lf\n", func, nowPos, res);
            nowPos += step;
            if (lastSig == 1)
            {
                lastSig = 0;
                break;
            }
        }
    }

    return 0;
}
