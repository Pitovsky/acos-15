//
//  main.c
//  task2
//
//  Created by Simon Fedotov on 19.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, const char * argv[]) {
    char* First = (char *) malloc(255 * sizeof(char));
    char* Second = (char *) malloc(255 * sizeof(char));
    First = argv[1];
    Second = argv[2];
    char* Check = (char *) malloc(255 * sizeof(char));
    char* ptr;
    while (1) {
        scanf("%s", Check);
        for(int i = 0; i < strlen(Check); ++i) {
            if((ptr = strchr(First, Check[i])) != NULL) {
                int Position = ptr - First;
                Check[i] = Second[Position];
            }
        
        }
        printf("%s\n", Check);
    }
    return 0;
}
