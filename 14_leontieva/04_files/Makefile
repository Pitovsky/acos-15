all: files 
files: files.o
	gcc files.o -o files
files.o: files.c
	gcc -c files.c -o files.o
files.c:

clean:
	rm files.o
