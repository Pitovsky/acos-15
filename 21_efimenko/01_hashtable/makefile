all: test1

test1: ht_test.o ht11.o
	gcc ht11.o ht_test.o -o test1

ht_test.o: ht_test.c
	gcc -std=c99 -c ht_test.c -o ht_test.o

ht11.o: ht11.c
	gcc -std=c99 -c ht11.c -o ht11.o

ht11.c:

ht_test.c:
