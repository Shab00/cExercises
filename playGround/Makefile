CC = gcc
CFLAGS= -Wall -g -DNDEBUG

all: ex1 ex2 ex3 ex4

ex1: ex1.c
	$(CC) $(CFLAGS) -o ex1 ex1.c

ex2: ex2.c
	$(CC) $(CFLAGS) -o ex2 ex2.c

ex3: ex3.c
	$(CC) $(CFLAGS) -o ex3 ex3.c

ex4: ex4.c
	$(CC) $(CFLAGS) -o ex4 ex4.c

clean:
	rm -f ex1 ex2 ex3 ex4
	rm -rf ex1.dSYM ex2.dSYM ex3.dSYM ex4.dSYM

rebuild:
	clean all
