all:result


result:Hash.o libmyhashtable.so
	LD_LIBRARY_PATH=/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable:$LD_LIBRARY_PATH
		gcc -L/Users/semenfedotov/Desktop/ACOS/ACOS_HashTable/ACOS_HashTable -Wall -o HashTable testfile.c -lmyhashtable


libmyhashtable.so: 
		gcc -shared -o libmyhashtable.so Hash.o

Hash.o:
	gcc -c -Wall -Werror -fPIC Hash.c


clean: rm Hash.o tesfile.o
