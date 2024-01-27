CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -L. -l:encriptor.so
TARGETS = encode_main decode_main task1 task2 task4 encriptor.so

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: all task3 clean

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

task4: task4.c
	$(CC) $(CFLAGS)  -o $@ $^

clean:
	rm -f $(TARGETS)
