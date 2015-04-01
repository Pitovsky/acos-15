all: myhash

myhash: main.o myhash.o 
	gcc main.o myhash.o -o myhash

main.o: main.c
	gcc -c main.c

myhash.o: myhash.c
	gcc -c myhash.c

clean:
	rm -rf *.o myhash

