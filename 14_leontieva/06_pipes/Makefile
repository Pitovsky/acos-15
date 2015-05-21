all:client server

client: client.o
	gcc client.o -o client
server: server.o
	gcc server.o -o server
client.o: client.c
	gcc -c client.c -o client.o
server.o:server.c
	gcc -c server.c -o server.o
server.c:

client.c:

clean:
	rm client.o
	rm server.o
