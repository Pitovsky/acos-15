#include <stdio.h>

int main(){
    long long a,b,c;
    a = 10;
    c = 15;

    asm("movq %1,%%rax\n"
        "mulq %2\n"
        "movq %%rax, %0\n"    
            
         :"=r"(b)
         :"d"(a),"r"(c)
         );

    printf("%lld %lld %lld \n", a, b, c);

    return 0;
}
