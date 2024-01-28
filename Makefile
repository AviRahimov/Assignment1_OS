CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -L. -l:encriptor.so
TARGETS = encode_main decode_main task1 task2 encriptor.so myzip myunzip

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: all task3 clean task4

all: task1 task2 task3 task4

encriptor.so: polyalphabetic_codec.c
	$(CC) $(CFLAGS) -shared -o $@ $^

encode_main: encode_main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

decode_main: decode_main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

task1: task1.c
	$(CC) $(CFLAGS)  -o $@ $^

task2: task2.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

task3: encriptor.so encode_main decode_main

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
