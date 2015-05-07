//
//  main.c
//  Task2
//
//  Created by Simon Fedotov on 07.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
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

int Frequency[256];

int main(int argc, const char * argv[]) {
    // insert code here...
    char* FilePath = malloc(1 << 10);
    FilePath = argv[1];
    
    int OurFD = open(FilePath, O_RDWR | O_CREAT | S_IRWXU, 0777);
    if(OurFD < 0) {
        perror("Error While opening File");
        exit(1);
    }
    
    struct stat statistic;
    if(fstat(OurFD, &statistic) < 0)
        printf("Can't get stats");
    int FileSize = statistic.st_size;
    
    char* FileAddress = (char*)mmap(NULL, FileSize, PROT_READ | PROT_WRITE, MAP_SHARED, OurFD, 0);
    
    printf("Bubu = %s\n", FileAddress);
    
    
    int Start, End;
    while(1) {
        memset(Frequency, 0, 256 * sizeof(int));
        int Res = scanf("%d %d", &Start, &End);
        if(Res == EOF) {
            printf("EveryThing is Allright\nGoodBye\n");
            exit(1);
        }
        int Length = End - Start;
        while (Start < End) {
            ++Frequency[FileAddress[Start]];
            ++Start;
        }
        
        
        double Result = 0;
        
        for(int i = 0; i < 255; ++i) {
            if(Frequency[i] != 0) {
                double Freq = (double)Frequency[i] / (double)Length;
                Result += -Freq * log(Freq);
            }
        }
        
        printf("Result is = %lf\n", Result);
        
    }
    
    munmap(FileAddress, FileSize);
    
    return 0;
}
