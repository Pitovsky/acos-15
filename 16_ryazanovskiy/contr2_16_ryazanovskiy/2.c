#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


int main(int argc, char** argv)
{
    assert(2 == argc);

    int fd = open(argv[1], O_RDONLY);

    assert(fd != -1);

    while (!feof(stdin))
    {
        int from = 0, to = 0;
        if (scanf("%i%i", &from, &to) != 2)
            break;
        const size_t len = to - from;
        char* fileAddr = (char*) mmap(NULL, to, PROT_READ, MAP_PRIVATE, fd, 0);  /// unfortunately can't use off here :(

        size_t stats[256] = {};
        for (size_t i = from; i < to; i++)
            stats[fileAddr[i]]++;

        double ans = 0;
        for (int i = 0; i < 256; i++)
            if (stats[i])
                ans += log2((double)len / (double) stats[i]) * (double) stats[i] / (double)len;

        printf("%lf\n", ans);

        munmap(fileAddr, len);
    }

    close(fd);
    return 0;
}
