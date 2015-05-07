//
//  main.c
//  Task1
//
//  Created by Simon Fedotov on 07.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int IPfunc(char* First, char* Second, char* Mask){
    return (inet_addr(First) & inet_addr(Mask)) == (inet_addr(Second) & inet_addr(Mask));
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    char* First = malloc(255);
    char* Second = malloc(255);
    char* Mask = malloc(255);
    
    scanf("%s %s %s", First, Second, Mask);
    
    printf("%d", IPfunc(First, Second, Mask));
    return 0;
}
