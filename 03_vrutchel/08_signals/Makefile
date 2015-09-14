all: run

run: main.o
	gcc main.o -o 08_signals -lm

main.o:
	gcc -c main.c

clean:
	 rm -rf *.o 08_signals
