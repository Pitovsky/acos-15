all:badTable

badTable:main.o libhashtable.so
	gcc main.o -L. -lhashtable -o badTable

main.o:main.c
	gcc -c main.c

libhashtable.so:hashTable.o
	gcc -shared -o libhashtable.so hashTable.o

hashTable.o:hashTable.c
	gcc -fPIC -c hashTable.c

main.c:

hashTable.c:

clean:
	rm hashTable.o main.o


