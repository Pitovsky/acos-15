all: main

main:main.c
	clang main.c -o main -std=gnu11 -Weverything -Wno-missing-prototypes -Wdollar-in-identifier-extension -D_DEFAULT_SOURCE

clean:
	rm -f main *.o
