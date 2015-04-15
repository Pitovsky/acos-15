//
//  main.c
//  Signals
//
//  Created by Simon Fedotov on 11.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <math.h>



void MakeSmth(char* Function, double start, double step, double deep) {
    double Dot = start + step;
    double Result = 0.0;
    double mult = 1.00000000;
    
    
    if(strcmp(Function, "sin") == 0) {
        while(1) {
            Result += sin(start);
            for(int Position = 1; Position <= deep; ++Position) {
                mult /= (double)Position;
                if(Position % 4 == 0) {
                    double coeff = mult * sin(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else if(Position % 4 == 2) {
                    double coeff = -mult * sin(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else if(Position % 4 == 1) {
                    double coeff = mult * cos(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else {
                    double coeff = -mult * cos(start);
                    Result += coeff * pow(Dot - start, Position);
                }
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
    }
    
    else if(strcmp(Function, "cos") == 0) {
        while(1) {
            Result += cos(start);
            for(int Position = 1; Position <= deep; ++Position) {
                mult /= (double)Position;
                if(Position % 4 == 0) {
                    double coeff = mult * cos(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else if(Position % 4 == 2) {
                    double coeff = -mult * cos(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else if(Position % 4 == 1) {
                    double coeff = -mult * sin(start);
                    Result += coeff * pow(Dot - start, Position);
                }
                else {
                    double coeff = mult * sin(start);
                    Result += coeff * pow(Dot - start, Position);
                }
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
    }
    
    else if(strcmp(Function, "exp") == 0) {
        while(1) {
            Result += exp(start);
            for(int Position = 1; Position <= deep; ++Position) {
                mult /= (double)Position;
                double coeff = mult * exp(start);
                Result += coeff * pow(Dot - start, Position);
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
    }
    
    else if(strcmp(Function, "ln") == 0) {
        while (1) {
            Result += log(start);
            for(int Position = 1; Position <= deep; ++Position) {
                if(Position % 2 == 0)
                    mult = -1.0 / (double)Position;
                else
                    mult = 1.0 / (double)Position;
                
                double coeff = mult * pow(start, Position);
                Result += coeff * pow(Dot - start, Position);
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
    }

}

static void function(int SignalNumber) {
    printf("\nNew command : \n");
    char* Function = malloc(255);
    int deep;
    double start, step;
    scanf("%s", Function);
    scanf("%lf %lf %d", &start, &step, &deep);
    MakeSmth(Function, start, step, deep);
    printf("\nbubu\n");
}


int main(int argc, const char * argv[]) {
    
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = function;
    action.sa_flags = SA_NODEFER;
    
    
    sigaction(SIGINT, &action, NULL);

    
    char* Function = malloc(255);
    int deep;
    double start, step;
    scanf("%s", Function);
    scanf("%lf %lf %d", &start, &step, &deep);
 
    MakeSmth(Function, start, step, deep);
    
    return 0;
}
