#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct root
{
    double r, i;
};

void print_root(struct root x)
{
    printf("( %+lf ; %+lf )", x.r, x.i);
}

int read_col(int a_flag, int b_flag, int c_flag, int _a, int _b, int _c, int complex)
{
    while(1)
    {
        int code = 1;
        double a = _a, b = _b, c = _c;

        if (a_flag == 0)
            code = scanf("%lf", &a);

        if ((b_flag == 0) && (code == 1))
            code = scanf("%lf", &b);

        if ((c_flag == 0) && (code == 1))
            code = scanf("%lf", &c);

        if(code == EOF)
        {
            break;
        }

        if(code !=  1)
        {
            return 1;
        }

        solve_equation(a, b, c, complex);
    }

    return 0;
}

int solve_equation(double a, double b, double c, int complex)
{
    struct root x1 = {0, 0}, x2 = {0, 0};

    if (a == 0)
    {
        if (b != 0)
        {
            x1.r = -1.*c/b;
            x1.i = 0;
            x2 = x1;
        }
        else
        {
            goto wrongInput;
        }
    }
    else
    {
        double d = (b * b) - 4 * a * c;

        if (d>=0)
        {
            x1.r = (-b + sqrt(d)) / (2 * a);
            x1.i = 0;

            x2.r = (-b - sqrt(d)) / (2 * a);
            x2.i = 0;
        }
        else
        {
            if (complex == 1)
            {
                x1.r = (-b ) / (2 * a);
                x1.i = sqrt(-1. * d) / (2 * a);

                x2.r = (-b) / (2 * a);
                x2.i = -1.*sqrt(-1. * d) / (2 * a);
            }
            else
            {
                goto wrongInput;
            }
        }
    }

    if (x1.r*x1.r + x1.i*x1.i > x2.r*x2.r + x2.i*x2.i)
    {
        struct root temp;
        temp = x1;
        x1 = x2;
        x2 = temp;
    }

    print_root(x1);
    printf("\t");
    print_root(x2);
    printf("\n");
    return 0;
wrongInput:
    fprintf(stderr, "Wrong command line arguments given.\n");
    return 1;
}

int main(int argc, char **argv)
{
    double a = 0, b = 0, c = 0;
    int a_flag = 0, b_flag = 0, c_flag = 0;
    int complex = 0;

    /*int argc = 5;
    char *argv[5]= {"nameprog", "-a", "0", "-c", "0"};*/


    size_t current = 1;
    while (current != argc)
    {
        if ((strcmp(argv[current], "-i") == 0) || (strcmp(argv[current], "--complex") == 0))
        {
            complex = 1;
            current++;
            continue;
        }

        if (strcmp(argv[current], "-a") == 0)
        {
            if (current + 1 == argc || a_flag == 1)
                goto wrongInput;

            char *err_ptr = argv[current + 1];
            a = strtod(argv[current + 1], &err_ptr);
            a_flag = 1;

            if (strcmp(err_ptr, "") == 0)
            {
                current += 2;
            }
            else
            {
                goto wrongInput;
            }

            continue;
        }

        if (strcmp(argv[current], "-b") == 0)
        {
            if (current + 1 == argc || b_flag == 1)
                goto wrongInput;

            char *err_ptr = argv[current + 1];
            b = strtod(argv[current + 1], &err_ptr);
            b_flag = 1;

            if (strcmp(err_ptr, "") == 0)
            {
                current += 2;
            }
            else
            {
                goto wrongInput;
            }

            continue;
        }

        if (strcmp(argv[current], "-c") == 0)
        {
            if (current + 1 == argc || c_flag == 1)
                goto wrongInput;

            char *err_ptr = argv[current + 1];
            c = strtod(argv[current + 1], &err_ptr);
            c_flag = 1;

            if (strcmp(err_ptr, "") == 0)
            {
                current += 2;
            }
            else
            {
                goto wrongInput;
            }

            continue;
        }

        current++;
    }

    //scanf("%lf%lf%lf", &a, &b, &c);

    if (a_flag && b_flag && c_flag)
    {
        return solve_equation(a, b, c, complex);
    }
    else
    {
        return read_col(a_flag, b_flag, c_flag, a, b, c, complex);
    }

wrongInput:
    fprintf(stderr, "Wrong command line arguments given.\n");
    return 1;
}

