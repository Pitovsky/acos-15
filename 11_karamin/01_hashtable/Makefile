all:hashtable

hashtable: HT.o test.o
	gcc HT.o test.o -o hashtable

HT.o: HT.c
	gcc -c HT.c -o HT.o
test.o: test.c
	gcc -c test.c -o test.o
HT.c:

test.c:

clean:
	rm test.o HT.o
