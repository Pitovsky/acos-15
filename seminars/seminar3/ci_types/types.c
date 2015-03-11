#include <stdio.h>

#define PI 3.141592653

#define MAX(a,b)  ((a) > (b) ? (a) : (b)) 

void printDebug(char * s){
#ifdef DLOG
    printf("dbg: %s\n" ,s);
#endif
    ;
}


typedef union {
    int whole;
    struct {
        union{
            short int whole;
        } lword;

        union {
            short int whole;
            struct{
                char half_byte1 : 4;
                char half_byte2 : 4;
                char half_byte3 : 4;
                char half_bete4 : 4;
            } parts;
        } hword;
    } parts;
} DWORD_type;

int  sumall(int num, ...){
    int* p = &num;
    int sum = 0;
    while(num > 0){
        printf("%d ", *p);
        --p;--p;
        printf("%d ", *p);
        sum += *p;
        num--;
    }
    return sum;
}

int main(){

    DWORD_type d;
    d.whole = 0xFF1422AA;

    printf("%d \t %x \n", d.whole, d.whole);
    printf("%hd \t %hx \n", (int)d.parts.hword.whole, (int)d.parts.hword.whole);
    printf("%hd \t %hx \n", (int)d.parts.lword.whole, (int)d.parts.lword.whole);

    int local = d.parts.hword.parts.half_byte1;
    printf("%d \t %x\n", local);

    printf("%f\n", PI);
    printf("%f\n", MAX(PI, 4));

    printDebug("abc");

    printf("%d\n", sumall(2, 1, 7));

    return 0;
}


