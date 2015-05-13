#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: filename\n");
        return 1;
    }
    struct stat st;
    stat(argv[1], &st);
    int filesize = st.st_size;
    int fd = open(argv[1], O_RDONLY);
    if (filesize <= 0 || fd < 0)
    {
        fprintf(stderr, "Error: cannot open this file\n");
        return 2;
    }
    char* infile = (char*)mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);//надо бы проверить на удачность операции
    while (1)
    {
        int begin = 0;
        int end = 0;
        if (scanf("%d", &begin) <= 0)
            break;
        scanf("%d", &end);
        int i = 0;
        double ans = 0;
        int letcount[255];
        for (i = 0; i < 255; ++i)
            letcount[i] = 0;
        for (i = begin; i < end; ++i)
            ++letcount[infile[i]];//ошбика: нужно использовать беззнаковый указатель
        for (i = 0; i < 255; ++i)
            if (letcount[i] != 0)
        {
            double letdens = 1.0*letcount[i]/(end - begin);
            ans = ans - letdens*log(letdens)/log(2);
        }
        printf("%lf\n", ans);
    }
    return 0;
}
