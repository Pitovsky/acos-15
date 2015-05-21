all: client

client: client.o
	gcc client.o -o client -lcrypto

client.o:
	gcc -c client.c
