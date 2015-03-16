all:prog_hash

prog_hash: hash.o main.o
	gcc hash.o main.o -o prog_hash

hash.o: hash.c
	gcc -c hash.c -o hash.o

main.o: main.c
	gcc -c main.c -o main.o

main.c:

hash.c:

clean:
	rm main.o hash.o
