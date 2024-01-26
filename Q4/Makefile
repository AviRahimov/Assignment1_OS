CC=gcc
CFLAGS=-Wall -g

.PHONY: all clean

all: myzip myunzip

myzip: myzip.o
	$(CC) $(CFLAGS) -o myzip myzip.o

myzip.o: myzip.c myzip.h
	$(CC) $(CFLAGS) -c myzip.c

myunzip: myunzip.o
	$(CC) $(CFLAGS) -o myunzip myunzip.o

myunzip.o: myunzip.c myunzip.h
	$(CC) $(CFLAGS) -c myunzip.c

clean:
	rm -f *.o myzip myunzip
