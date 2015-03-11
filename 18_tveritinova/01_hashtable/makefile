all: rez
rez: main.o libmy.so
	gcc main.o -L. -lmy -o rez
main.o: main.c
	gcc -std=c99 -c main.c
main.c:
libmy.so: lib.o
	gcc -shared -o libmy.so lib.o
lib.o: lib.c
	gcc -fPIC -std=c99 -c lib.c
lib.c:
