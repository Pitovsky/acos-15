//  equationSolver.c
//  02_shell
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include "equationSolver.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

static const NSInteger memAllocationError = 1000;
static const NSInteger usageCodeError = 1001;

void throwException(NSInteger code)
{
    perror("----------WASTED!!1!!1!1----------\n");
    switch (code)
    {
        case memAllocationError:
            perror("MEMORY ALLOCATION ERROR\n");
            break;
            
        case usageCodeError:
            perror("WRONG USAGE\n");
            break;
            
        default:
            perror("EXCEPTION WITH NO REASON\n");
            break;
    }
    perror("----------WASTED1!111!1!----------\n");
    
    exit(EXIT_FAILURE);
}

GeneralEquation *processInput(NSInteger argc, const char **argv)
{
    GeneralEquation *equation = NULL;
    if (!(equation = (GeneralEquation *)calloc(sizeof(GeneralEquation), 1)))
        throwException(memAllocationError);
    equation->mask = 00000000;
    
    for (NSInteger i = 1; i < argc; i++)
    {
        NSInteger dFactorBuffer = 0;
        
        if (strcmp(argv[i], "-a") == 0)
        {
            equation->mask |= aFactorMask;
            
            if (i+1 == argc)
                throwException(usageCodeError);
            
            if (!(equation->aFactor = atof(argv[i+1])))
                throwException(usageCodeError);
            i++;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            equation->mask |= bFactorMask;
            
            if (i+1 == argc)
                throwException(usageCodeError);
            
            if (!(equation->bFactor = atof(argv[i+1])))
                throwException(usageCodeError);
            i++;
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            equation->mask |= cFactorMask;
            
            if (i+1 == argc)
                throwException(usageCodeError);
            
            if (!(equation->cFactor = atof(argv[i+1])))
                throwException(usageCodeError);
            i++;
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--complex") == 0)
        {
            equation->mask |= complexMask;
        }
        else if ((dFactorBuffer = atof(argv[i])))
        {
            equation->dFactor += dFactorBuffer;
        }
        else
        {
            throwException(usageCodeError);
        }
    }
    
    return equation;
}

void printConfiguration(GeneralEquation *equation)
{
    if (!equation)
    {
        throwException(memAllocationError);
    }
        
    if (!equation->mask)
    {
        printf("USING NO CONFIGURATION\n");
        return;
    }
    
    printf("USING FOLLOWING CONFIGURATION:\n");
    
    if (equation->mask & aFactorMask)
    {
        printf("  'a' FACTOR - %.2f\n", equation->aFactor);
    }
    
    if (equation->mask & bFactorMask)
    {
        printf("  'b' FACTOR - %.2f\n", equation->bFactor);
    }
    
    if (equation->mask & cFactorMask)
    {
        printf("  'c' FACTOR - %.2f\n", equation->cFactor);
    }
    
    if (equation->mask & complexMask)
    {
        printf("  COMPLEX SOLUTION\n");
    }
    
    printf("--------------------------------\n");
    
    printf("TYPE 'q' TO EXIT\n");
    
    printf("--------------------------------\n");
    
    return;
}

Equation scanEquation(GeneralEquation *generalEquation)
{
    Equation equation;
    printf("\n");

    if (!(generalEquation->mask & aFactorMask))
    {
        printf("TYPE A : ");
        scanf("%f", &equation.aFactor);
    }
    else
    {
        equation.aFactor = generalEquation->aFactor;
    }
    
    if (!(generalEquation->mask & bFactorMask))
    {
        printf("TYPE B : ");
        scanf("%f", &equation.bFactor);
    }
    else
    {
        equation.bFactor = generalEquation->bFactor;
    }
    
    if (!(generalEquation->mask & cFactorMask))
    {
        printf("TYPE C : ");
        scanf("%f", &equation.cFactor);
    }
    else
    {
        equation.cFactor = generalEquation->cFactor;
    }
    
    equation.dFactor = generalEquation->dFactor;
    
    printf("(%.2f)x^2 + (%.2f)x + (%.2f) = %.2f\n", equation.aFactor, equation.bFactor, equation.cFactor, equation.dFactor);
    
    return equation;
}

void solveEquation(Equation equation, NSInteger isComplex)
{
    equation.cFactor -= equation.dFactor;
    
    CGFloat D = equation.bFactor*equation.bFactor - 4*equation.cFactor*equation.aFactor;
    
    if (!isComplex && D < 0)
    {
        perror("NO SOULTION\n");
    }
    else if ((isComplex && D < 0))
    {
        printf("(%.2f - sqrt(%.2f))/%.2f\t(%.2f + sqrt(%.2f))/%.2f\n", equation.bFactor, D, 2*equation.aFactor, equation.bFactor, D, 2*equation.aFactor);
    }
    else if (D >= 0)
    {
        printf("%.2f\t%.2f\n", (equation.bFactor - sqrt(D))/2*equation.aFactor, (equation.bFactor + sqrt(D))/2*equation.aFactor);
    }
}

