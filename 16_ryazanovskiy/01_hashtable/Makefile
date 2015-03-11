all: main
	./main

main:main.c
	clang main.c -o main -std=c11 -Weverything \
		-Wno-missing-prototypes -Wno-dollar-in-identifier-extension -Wno-unused-macros -D_DEFAULT_SOURCE -g

clean:
	rm -f main
