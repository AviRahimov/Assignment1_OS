CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = inf_rec uninit_val

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: clean task1

task1: inf_rec uninit_val

inf_rec: inf_rec.c
	$(CC) $(CFLAGS)  -o $@ $^


uninit_val: uninit_val.c
	$(CC) $(CFLAGS)  -o $@ $^

clean: 
	rm -f *.o $(TARGETS)