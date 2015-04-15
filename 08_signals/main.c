#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

sig_atomic_t shouldStopCalculating = 0;
void sigIntHandler(int signal)
{
    shouldStopCalculating = 1;
}

void printUsage(FILE *stream)
{
    fprintf(stream, "!111!!!!!11 USAGE: <func> <start> <step> <deep>\n");
    exit(1);
}

typedef enum
{
    sinx,
    cosx,
    lnx,
    ex
} Function;

typedef struct
{
    Function function;
    double initialPoint;
    double step;
    size_t deep;
    
} Equation;

void printEquation(Equation equation)
{
    printf("%d %lf %lf %zu\n", equation.function, equation.initialPoint, equation.step, equation.deep);
}

Equation getEquation()
{
    char buf[256];
    char c = 0;
    for (size_t i = 0; (c = getchar()) != '\n' && c != EOF; i++)
    {
        buf[i] = c;
        buf[i+1] = '\0';
    }
    
    Equation equation;
    
    char *occurrence = NULL;
    if ((occurrence = strstr(buf, "cos")) && occurrence == buf)
    {
        equation.function = cosx;
        occurrence += strlen("cos") + 1;
    }
    else if ((occurrence = strstr(buf, "sin")) && occurrence == buf)
    {
        equation.function = sinx;
        occurrence += strlen("sin") + 1;
    }
    else if ((occurrence = strstr(buf, "exp")) && occurrence == buf)
    {
        equation.function = ex;
        occurrence += strlen("exp") + 1;
    }
    else if ((occurrence = strstr(buf, "ln")) && occurrence == buf)
    {
        equation.function = lnx;
        occurrence += strlen("ln") + 1;
    }
    else 
        printUsage(stdout);
    
    if (sscanf(occurrence, "%lf %lf %zu", &equation.initialPoint, &equation.step, &equation.deep) != 3)
        printUsage(stdout);
    
    return equation;
}

void taylorForEquation(Equation equation, double current)
{
    if (equation.function == cosx)
    {
        for (size_t i = 0; i < equation.deep; i++)
        {
            printf("(cos(%f + pi*%zu/2)/%zu!) * (x - %lf)^%zu", current, i, i, current, i);
            
            if (i != equation.deep - 1 && equation.deep > 1)
                printf(" + ");
        }
        printf("\n");
    }
    
    if (equation.function == sinx)
    {
        for (size_t i = 0; i < equation.deep; i++)
        {
            printf("(sin(%f + pi*%zu/2)/%zu!) * (x - %lf)^%zu", current, i, i, current, i);
            
            if (i != equation.deep - 1 && equation.deep > 1)
                printf(" + ");
        }
        printf("\n");
    }
    
    if (equation.function == lnx)
    {
        for (size_t i = 0; i < equation.deep; i++)
        {
            printf("(((-1)^(%zu - 1)(%zu - 1))/(%lf)^(%zu)*%zu!) * (x - %lf)^%zu", i, i, current, i, i, current, i);
            
            if (i != equation.deep - 1 && equation.deep > 1)
                printf(" + ");
        }
        printf("\n");
    }
    
    if (equation.function == ex)
    {
        for (size_t i = 0; i < equation.deep; i++)
        {
            printf("1/%zu!) * (x - %lf)^%zu", i, current, i);
            
            if (i != equation.deep - 1 && equation.deep > 1)
                printf(" + ");
        }
        printf("\n");
    }
}

int main()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &sigIntHandler;
    sigaction(SIGINT, &sa, NULL);
    
startPoint: ;
    
    Equation equation = getEquation();
    
    for (double x = equation.initialPoint; !shouldStopCalculating; x += equation.step)
    {
        taylorForEquation(equation, x);
    }
    
    shouldStopCalculating = 0;
    goto startPoint;
    
    return 0;
}
