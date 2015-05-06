all: runServer

runServer: main.o
	gcc main.o -o Server -lpthread

main.o:
	gcc -c main.c

clean:
	rm -rf *.o Server
