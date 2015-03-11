all: dynamic run clean

dynamic:
	gcc -fPIC -c table.c
	gcc -fPIC -c tests.c
	gcc -shared -o libdynamic.so table.o tests.o
	gcc main.c -L. -ldynamic -o resultdyn

run:
	./resultdyn

clean:
	rm -rf ./*.o
	rm -rf ./resultdyn
