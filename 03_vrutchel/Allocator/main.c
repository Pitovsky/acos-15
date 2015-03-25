#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

int main()
{
    piece_count = 0;
    piece_first = sbrk(0);

    printf("%d %p\n", sizeof(SPiece), piece_first);

    printf("%d %p\n", piece_count, sbrk(0));

    piece_create();
    piece_autoclean();
    piece_autoclean();
    piece_autoclean();

    const int N = 20;
    void * ptrs[N];
    int i;
    for(i = 0; i < N; i++) {
        ptrs[i] = my_malloc(30*1024);
    }
    for(i = 0; i < N; i+=2) {
        my_free(ptrs[i]);
    }
    for(i = 0; i < N; i++) {
        ptrs[i] = my_malloc(30*1024);
    }

    for(i = 0; i < N; i++) {
        ptrs[i] = my_realloc(ptrs[i], 40*1024);
    }

    for(i = 0; i < N; i++) {
        my_free(ptrs[i]);
    }

    return 0;
}
