all: run

run: main.o alloc.o alloc_int.o
	gcc main.o alloc.o alloc_int.o -o Allocator

main.o: main.c
	gcc -c main.c

alloc.o: alloc.c
	gcc -c alloc.c

alloc_int.o: alloc_int.c
	gcc -c alloc_int.c

clean:
	rm -rf *.o Allocator
