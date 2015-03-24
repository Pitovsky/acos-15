//
//  main.c
//  Task1
//
//  Created by Simon Fedotov on 19.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

unsigned short int* convert(unsigned int* myNum) {
    unsigned short x = 1;
    char* p =(char *) &x;
    if(*p == 1) { //LittleEndian
        return (unsigned short int *) myNum;
    }
    else {//BigEndian
        return (unsigned short int *) myNum + 1;
    }
}

int main(int argc, const char * argv[]) {
    unsigned int myNum;
    scanf("%u", &myNum);
    printf("%p\n", myNum);
    unsigned short int* res = convert(&myNum);
    printf("%p\n", res);
    printf("%u", *res);
    return 0;
}
