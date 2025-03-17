CC = gcc
CFLAGS= -Wall -g -DNDEBUG

all: ex1 

ex1: ex1.c
	$(CC) $(CFLAGS) -o ex1 ex1.c

clean:
	rm -f ex1
	rm -rf ex1.dSYM

rebuild:
	clean all
