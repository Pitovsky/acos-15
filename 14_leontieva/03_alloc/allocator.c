/*Sorry, i haven't enough time, hope you give me status "in process"*/
#include <unistd.h>
#define mem_part 8 

typedef struct node {
    size_t size;
    struct node* prev;
    struct node* next;
    void *ptr;
    int free;
    char* key;
} node;

node* first = NULL;

node* free_space(node** last, size_t size) {
    node* b = first;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

node* reserve_space(node* last, size_t s) {
    node* b = (node*)sbrk(0);
    if (sbrk(s + mem_part) == (void*)-1)
        return NULL;
    b->size = s;
    b->free = 0;
    b->ptr = b->key;
    b->next = NULL;
    if (last) last->next = b;
    b->prev = last;
    return b;
}
void split(node* b, size_t s) {
    node* newmem = (node*)(b->key + s);
    newmem->size = b->size - mem_part - s;
    newmem->free = 1;
    newmem->next = b->next;
    newmem->ptr = newmem->key;
    newmem->prev = b;
    b->next = newmem;
    b->size = s;
}
void *malloc(size_t size) {
    node* last; node*b;
    if (first) {
        last = first;
        b = free_space(&last, size);
        if (b) {
            if (b->size >= mem_part + 8 + size)
                split(b, size);
            b->free = 0;
        } else {
            b = reserve_space(last, size);
            if (!b) return NULL;
        }
    } else {
        b = reserve_space(NULL, size);
        if (!b) return NULL;
        first = b;
    }
    return b->key;
}

void *calloc(size_t number, size_t size) {
    int i;
    size_t *arr = (size_t*)malloc(number * size);
    if (arr) {
        for (i=0; i<(number * size); i++)
            arr[i] = 0;
    }
    return arr;
}

int valid_addr(void *p) {
    if (first) {
        if (p > first && p < sbrk(0));
        return p == ((node*)(p-mem_part))->ptr;
    }
    return 0;
}

node* merge(node* b) {
    b->size = b->size + mem_part + b->next->size;
    b->next = b->next->next;
    if (b->next)
        b->next->prev = b;
    return b;
}

void free(void *p) {
    if (valid_addr(p)) {
        node* b = (node*)(p-mem_part);
        b->free = 1;
        if (b->prev && b->prev->free)
            b = merge(b->prev);

        if (b->next && b->next->free)
            b = merge(b);
        else if (b->next == NULL) {
            if (b->prev)
                b->prev->next = NULL;
            brk(b);
        }
    }
}

int main(){
char *obrk;
char *naddr;
char* nbrk;

//current break-address
obrk = sbrk(0);
printf("Current b.a. = %x\n", obrk);

//reserve 32 b

naddr = malloc(32);
printf("malloc(32)\n");
//new break-address
nbrk = sbrk(0);
printf("New addres malloced = %x, b.a = %x, increase for  %d bytes\n", naddr,nbrk, nbrk - obrk);
return 0;

}
