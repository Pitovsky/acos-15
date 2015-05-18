all:make

make:main.o libht.so
	gcc main.o -L -lht -o make
main.o:main.c
	gcc -c main.c


libht.so:HashTable.o
	gcc -shared -o libht.so HashTable.o



HashTable.o:HashTable.c
	gcc -fPIC -c HashTable.c

HashTable.c:

main.c:
