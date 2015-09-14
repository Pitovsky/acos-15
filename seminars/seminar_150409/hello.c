#include <unistd.h>

char str[]="Hello world!";

int main(){
    write(1, str, sizeof(str));
    _exit(0);
}
