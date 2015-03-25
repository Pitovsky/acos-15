all:result

result:main.o my-allocator.o
	gcc main.o my-allocator.o -o result

main.o:main.c
	gcc -c main.c

my-allocator.o:my-allocator.c
	gcc -c my-allocator.c

main.c:

my-allocator.c:
