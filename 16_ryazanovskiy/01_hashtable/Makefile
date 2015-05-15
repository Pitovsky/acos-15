all: main

FLAGS=-std=gnu11 -Weverything 		-Wno-missing-prototypes -Wno-dollar-in-identifier-extension -Wno-unused-macros -D_DEFAULT_SOURCE

main:main.o hashset.o
	clang main.o hashset.o -o main

main.o:main.c
	clang -c main.c -o main.o $(FLAGS)

hashset.o:hashset.c
	clang -c hashset.c -o hashset.o $(FLAGS)

clean:
	rm -f main *.o
