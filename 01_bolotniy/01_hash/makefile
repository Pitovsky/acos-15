all:result

result:main.o libhash.so
	gcc main.o -L. -lhash -o result

libhash.so:table.o
	gcc -shared -o libhash.so table.o

main.o:main.c
	gcc -c main.c

table.o:table.c
	gcc -fPIC -c table.c

table.c:

main.c:

