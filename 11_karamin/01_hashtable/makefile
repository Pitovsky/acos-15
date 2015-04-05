all: hashtabletest clean
hashtabletest: test.o
	gcc test.o -o hashtabletest
test.o : test.c
	gcc -c test.c -o test.o
clean:
	rm test.o
