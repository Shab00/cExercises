CC = gcc
CFLAGS= -Wall -g -DNDEBUG

all: ex1 ex2 ex3

ex1: ex1.c
	$(CC) $(CFLAGS) -o ex1 ex1.c

ex2: ex2.c
	$(CC) $(CFLAGS) -o ex2 ex2.c

ex3: ex3.c
	$(CC) $(CFLAGS) -o ex3 ex3.c

clean:
	rm -f ex1 ex2 ex3
	rm -rf ex1.dSYM ex2.dSYM ex3.dSYM

rebuild:
	clean all
