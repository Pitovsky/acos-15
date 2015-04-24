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

sig_atomic_t FlagToBreak = 0;



void MakeSmth(char* Function, double start, double step, int deep) {
    double Dot = start + step;
    double Result = 0.0;
    double mult = 1.00000000;
    
    
    if(strcmp(Function, "sin") == 0) {
        double Coeffs[deep + 1];
        
        Coeffs[0] = sin(start);
        for(int Position = 1; Position <= deep; ++Position) {
            mult /= (double)Position;
            if(Position % 4 == 0)
                Coeffs[Position] = mult * sin(start);
            
            else if(Position % 4 == 2)
                Coeffs[Position] = -mult * sin(start);
            
            else if(Position % 4 == 1)
                Coeffs[Position] = mult * cos(start);
            
            else
               Coeffs[Position] = -mult * cos(start);
            
        }

        
        while(!FlagToBreak) {
            Result += sin(start);
            for(int Position = 1; Position <= deep; ++Position) {
                mult /= (double)Position;
                if(Position % 4 == 0)
                    Result += Coeffs[Position] * pow(Dot - start, Position);
                
                else if(Position % 4 == 2)
                    Result += Coeffs[Position] * pow(Dot - start, Position);
                
                else if(Position % 4 == 1)
                    Result += Coeffs[Position] * pow(Dot - start, Position);
                
                else
                    Result += Coeffs[Position] * pow(Dot - start, Position);
                
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
        FlagToBreak = 0;

    }
    
    else if(strcmp(Function, "cos") == 0) {
        double Coeffs[deep + 1];
        Coeffs[0] = cos(start);
        for(int Position = 1; Position <= deep; ++Position) {
            mult /= (double)Position;
            if(Position % 4 == 0)
                 Coeffs[Position] = mult * cos(start);
            
            else if(Position % 4 == 2)
                Coeffs[Position] = -mult * cos(start);
            
            else if(Position % 4 == 1)
                Coeffs[Position] = -mult * sin(start);
            
            else
                Coeffs[Position] = mult * sin(start);
            
        }

        
        while(!FlagToBreak) {
            for(int Pos = 0; Pos <= deep; ++Pos) {
                if(Pos % 4 == 0)
                    Result += Coeffs[Pos] * pow(Dot - start, Pos);
                
                else if(Pos % 4 == 2)
                    Result += Coeffs[Pos] * pow(Dot - start, Pos);
                
                else if(Pos % 4 == 1)
                    Result += Coeffs[Pos] * pow(Dot - start, Pos);
                
                else
                    Result += Coeffs[Pos] * pow(Dot - start, Pos);
                
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
        FlagToBreak = 0;

    }
    
    else if(strcmp(Function, "exp") == 0) {
        double Coeffs[deep + 1];
        Coeffs[0] = exp(start);
        for(int Position = 1; Position <= deep; ++Position) {
            mult /= (double)Position;
            Coeffs[Position] = mult * exp(start);
        }

        while(!FlagToBreak) {
            for(int Pos = 0; Pos <= deep; ++Pos) {
                Result += Coeffs[Pos] * pow(Dot - start, Pos);
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
        FlagToBreak = 0;

    }
    
    else if(strcmp(Function, "ln") == 0) {
        double Coeffs[deep + 1];
        Coeffs[0] = log(start);
        for(int Position = 1; Position <= deep; ++Position) {
            if(Position % 2 == 0)
                mult = -1.0 / (double)Position;
            else
                mult = 1.0 / (double)Position;
            
             Coeffs[Position] = mult * pow(start, Position);
        }
        
        while (!FlagToBreak) {
            for(int Pos = 0; Pos <= deep; ++Pos) {
                Result += Coeffs[Pos] * pow(Dot - start, Pos);
            }
            printf("Result is = %lf\n", Result);
            Dot += step;
            Result = 0;
            mult = 1;
            sleep(1);
        }
    }
    
    FlagToBreak = 0;
}



void SIGINTHandler(int SignalNumber) {
    FlagToBreak = 1;
    printf("We've got a Signal\n");
}


int main(int argc, const char * argv[]) {
    
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = SIGINTHandler;

    
    sigaction(SIGINT, &action, NULL);

    
//    char* Function = malloc(255);
//    int deep;
//    double start, step;
//    scanf("%s", Function);
//    scanf("%lf %lf %d", &start, &step, &deep);
    
    while (1) {
        printf("\nNew command : \n");
        char* Function = malloc(255);
        int deep;
        double start, step;
        scanf("%s", Function);
        scanf("%lf %lf %d", &start, &step, &deep);
        MakeSmth(Function, start, step, deep);
        printf("\nbubu\n");
    }
 
    
    return 0;
}
