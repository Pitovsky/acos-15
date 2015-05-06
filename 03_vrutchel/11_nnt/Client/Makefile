all: runClient

runClient: main.o
	gcc main.o -o Client -lpthread

main.o:
	gcc -c main.c

clean:
	rm -rf *.o Client
