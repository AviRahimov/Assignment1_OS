CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = task1 uninit_val

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: clean

task1: task1.c
	$(CC) $(CFLAGS)  -o $@ $^


uninit_val: uninit_val.c
	$(CC) $(CFLAGS)  -o $@ $^

clean: 
	rm -f *.o $(TARGETS)