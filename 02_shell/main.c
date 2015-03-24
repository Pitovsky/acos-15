//
//  main.c
//  02_shell
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equationSolver.h"

NSInteger main(NSInteger argc, const char **argv)
{
    GeneralEquation *generalEquation = processInput(argc, argv);
    printConfiguration(generalEquation);
    
    while (1)
    {
        printf("ENTER FACTOR(S) : ");
        Equation equation = scanEquation(generalEquation);
        solveEquation(equation, generalEquation->mask & complexMask);
        
        char c;
        c = getchar();
        if (c == 'q')
            break;
    }
    
    free(generalEquation);
    return 0;
}
