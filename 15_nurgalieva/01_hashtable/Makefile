all: hashtable
hashtable: create.o table.o
	gcc create.o table.o -o hashtable
create.o: create.c
	gcc -c create.c -o create.o
table.o: table.c
	gcc -c table.c -o table.o
table.c:
create.c:

clear:
	rm create.o table.o