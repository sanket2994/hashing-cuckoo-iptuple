CC=gcc

default: server library clean

library: hash.o
	ar -crv libhash.a hash.o

server: library server.o
	$(CC) -o server server.o -lhash -lpthread -L.

server.o: server.c
	$(CC) -c server.c

hash.o: hash.c hash.h
	$(CC) -c hash.c

clean:hash.o server.o
	rm -rf *.o
