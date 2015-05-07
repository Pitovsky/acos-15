all: HTTP_SERVER clean
HTTP_SERVER: serv.o
	gcc -pthread serv.o -o HTTP_SERVER
serv.o: serv.c
	gcc -c serv.c -o serv.o
serv.c:
clean: 
	rm serv.o
