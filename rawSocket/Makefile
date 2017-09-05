CC=gcc

default: server library clean

library: hash.o
	ar -crv libhash.a hash.o

server: library rawserver.o
	$(CC) -o rawserver rawserver.o -lhash -lpthread -L.

rawserver.o: rawserver.c
	$(CC) -c rawserver.c

hash.o: hash.c hash.h
	$(CC) -c hash.c

clean:hash.o rawserver.o
	rm -rf *.o
