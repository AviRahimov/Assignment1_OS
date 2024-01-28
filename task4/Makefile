CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -L. -l:encriptor.so
TARGETS = myzip myunzip output

.PHONY: clean task4

task4: myzip myunzip

myzip: myzip.o
	$(CC) $(CFLAGS) -o myzip myzip.o

myzip.o: myzip.c
	$(CC) $(CFLAGS) -c myzip.c

myunzip: myunzip.o
	$(CC) $(CFLAGS) -o myunzip myunzip.o

myunzip.o: myunzip.c
	$(CC) $(CFLAGS) -c myunzip.c

clean: 
	rm -f *.o $(TARGETS)