CC = gcc
CFLAGS= -Wall -g -DNDEBUG

all: ex1 ex2 ex3 ex4 pyr ex5 ex6 ex7

ex1: ex1.c
	$(CC) $(CFLAGS) -o ex1 ex1.c

ex2: ex2.c
	$(CC) $(CFLAGS) -o ex2 ex2.c

ex3: ex3.c
	$(CC) $(CFLAGS) -o ex3 ex3.c

ex4: ex4.c
	$(CC) $(CFLAGS) -o ex4 ex4.c

pyr: pyr.c
	$(CC) $(CFLAGS) -o pyr pyr.c

ex5: ex5.c
	$(CC) $(CFLAGS) -o ex5 ex5.c

ex6: ex6.c
	$(CC) $(CFLAGS) -o ex6 ex6.c

ex7: ex7.c
	$(CC) $(CFLAGS) -o ex7 ex7.c

clean:
	rm -f ex1 ex2 ex3 ex4 pyr ex5 ex6 ex7
	rm -rf ex1.dSYM ex2.dSYM ex3.dSYM ex4.dSYM pyr.dSYM ex5.dSYM ex6.dSYM ex7.dSYM

rebuild:
	clean all
