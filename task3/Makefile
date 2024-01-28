CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -L. -l:encriptor.so
TARGETS = encode_main decode_main encriptor.so

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: task3 clean

task3: encriptor.so encode_main decode_main

encriptor.so: polyalphabetic_codec.c
	$(CC) $(CFLAGS) -shared -o $@ $^

encode_main: encode_main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

decode_main: decode_main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o $(TARGETS)