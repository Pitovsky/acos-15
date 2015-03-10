all:result

result:main.o libmht.so
	gcc main.o -L. -lmht -o result
libmht.so:Hash-Table.o
	gcc -shared -o libmht.so Hash-Table.o

main.o:main.c
	gcc -std=c99 -c main.c

Hash-Table.o:Hash-Table.c
	gcc -std=c99 -fPIC -c Hash-Table.c

Hash-Table.c:

main.c:
