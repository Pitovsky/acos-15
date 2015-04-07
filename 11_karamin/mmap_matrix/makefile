all: matrixtobin mmap clean
matrixtobin: matrixtobin.o
	gcc matrixtobin.o -o matrixtobin
mmap: mmap.o
	gcc mmap.o -o mmap
matrixtobin.o: matrixtobin.c
	gcc -c matrixtobin.c - o matrixtobin.o
mmap.o: mmap.c
	gcc -c mmap.c -o mmap.o
matrixtobin.c:
mmap.c:
clean:
	rm *.o
