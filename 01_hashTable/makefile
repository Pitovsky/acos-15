all:hash

hash:main.o libht.so
	        gcc main.o -L. -lht -o hash
libht.so:hash.o
	        gcc -shared hash.o  -o libht.so

main.o:main.c
	        gcc -std=c99 -c main.c

hash.o:hash.c
	        gcc -std=c99 -fPIC -c hash.c

hash.c:

main.c:

clean:
		rm *.o *.so hash
