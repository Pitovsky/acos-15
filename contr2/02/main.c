#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <math.h>

float ent(float *p)
{
    int i;
    float entropy;
    
    entropy = 0.0;
    for(i = 0; i < 255; i++)
        if(p[i] > 0.0)
            entropy -= p[i] * (float) log( (double) p[i]);
    
    return entropy;
}

int qty(const char *data, int size, char c)
{
    int qty = 0;
    for (int i = 0; i < size; i++)
    {
        if (data[i] == c)
        {
            qty++;
        }
    }
    return qty++;
}

int main(int argc, char **argv)
{
    
    int fd = 0;
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Can't open a file");
        exit(1);
    }
    
    struct stat fileInfo;
    lstat(argv[1], &fileInfo);
    const char *data;
    if ((data = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("Can't mmap a file");
        exit(1);
    }
    
    int intervals[2];
    
    for (;;)
    {
        if (scanf("%d %d", &intervals[0], &intervals[1]) == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        
        int distance = intervals[1] - intervals[0];
        float p[255] = {0};
        for (int i = 0; i < 255; i++)
        {
            p[i] = qty(data + intervals[0], distance, i) / distance;
        }
        
        printf("%f\n", ent(p));
    }
    
    
    
    return 0;
}