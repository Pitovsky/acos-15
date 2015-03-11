all: test

test: test.o libhashtable.so
	gcc test.o -L. -lhashtable -o test

libhashtable.so: hashtable.o
	gcc -shared -o libhashtable.so hashtable.o

hashtable.o: hashtable.c
	gcc -fPIC -c hashtable.c

hashtable.c:

test.o:
	gcc -c test.c

test.c:
	
clean:
	rm *.o
