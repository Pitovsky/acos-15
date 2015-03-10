all:HashTable

hashtable: HT.o main.o
	gcc HT.o main.o -o HashTable

HT.o: HT.c
	gcc -c HT.c -o HT.o
main.o: main.c
	gcc -c main.c -o main.o
HT.c:

main.c:

clean:
	rm main.o HT.o