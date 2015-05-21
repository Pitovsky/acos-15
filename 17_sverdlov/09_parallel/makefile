all: parallel

parallel: parallel.c
	gcc -std=c99 -D_XOPEN_SOURCE=500 main.c -lrt -lm -pthread

parallel.c:

clean:
	rm *.swp parallel
