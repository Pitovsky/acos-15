//
//  equationSolver.h
//  02_shell
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <stdio.h>

typedef int NSInteger;
typedef unsigned int NSUInteger;
typedef float CGFloat;

static short complexMask = 00001000;
static short aFactorMask = 00000100;
static short bFactorMask = 00000010;
static short cFactorMask = 00000001;

typedef struct
{
    CGFloat aFactor;
    CGFloat bFactor;
    CGFloat cFactor;
    CGFloat dFactor;
    
    short mask;
} GeneralEquation;

typedef struct
{
    CGFloat aFactor;
    CGFloat bFactor;
    CGFloat cFactor;
    CGFloat dFactor;
} Equation;

GeneralEquation *processInput(NSInteger argc, const char **argv);
void printConfiguration(GeneralEquation *equation);
Equation scanEquation(GeneralEquation *generalEquation);
void solveEquation(Equation equation, NSInteger isComplex);