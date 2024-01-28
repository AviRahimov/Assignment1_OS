CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -L. -l:encriptor.so
TARGETS = task2 encriptor.so

ifeq ($(DEBUG), 1)
  CFLAGS += -g3 -DDEBUG
endif

.PHONY: clean

task2: task2.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f *.o $(TARGETS)