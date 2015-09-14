all:run

run: main.o parser.o
	gcc main.o parser.o -o Parser

main.o: main.c
	gcc -c main.c

parser.o: parser.c
	gcc -c parser.c

clear:
	rm -rf *.o Parser