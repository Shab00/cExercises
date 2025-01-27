CC = gcc
CFLAGS= -Wall -g -DNDEBUG

all: ex1 ex3 ex7 ex8 ex9 ex10 vowel ex11 ex12 ex13 ex14 ex15 ex16 extraCred ex17 ex18 ex19

ex1: ex1.c
	$(CC) $(CFLAGS) -o ex1 ex1.c

ex3: ex3.c
	$(CC) $(CFLAGS) -o ex3 ex3.c

ex7: ex7.c
	$(CC) $(CFLAGS) -o ex7 ex7.c

ex8: ex8.c
	$(CC) $(CFLAGS) -o ex8 ex8.c

ex9: ex9.c
	$(CC) $(CFLAGS) -o ex9 ex9.c

ex10: ex10.c
	$(CC) $(CFLAGS) -o ex10 ex10.c

ex11: ex11.c
	$(CC) $(CFLAGS) -o ex11 ex11.c

vowel: vowel.c
	$(CC) $(CFLAGS) -o vowel vowel.c

ex12: ex12.c 
	$(CC) $(CFLAGS) -o ex12 ex12.c

ex13: ex13.c
	$(CC) $(CFLAGS) -o ex13 ex13.c

ex14: ex14.c
	$(CC) $(CFLAGS) -o ex14 ex14.c

ex15: ex15.c
	$(CC) $(CFLAGS) -o ex15 ex15.c

ex16: ex16.c
	$(CC) $(CFLAGS) -o ex16 ex16.c

extraCred: extraCred.c
	$(CC) $(CFLAGS) -o extraCred extraCred.c

ex17: ex17.c
	$(CC) $(CFLAGS) -o ex17 ex17.c

ex18: ex18.c
	$(CC) $(CFLAGS) -o ex18 ex18.c

ex19: ex19.c
	$(CC) $(CFLAGS) -o ex19 ex19.c

clean:
	rm -f ex1 ex3 ex7 ex8 ex9 ex10 vowel ex11 ex12 ex13 ex14 ex15 ex16 extraCred ex17 ex18 ex19
	rm -rf ex1.dSYM ex3.dSYM ex7.dSYM ex8.dSYM ex9.dSYM ex10.dSYM vowel.dSYM ex11.dSYM ex12.dSYM ex13.dSYM ex14.dSYM ex15.dSYM ex16.dSYM extraCred.dSYM ex17.dSYM ex18.dSYM ex19.dSYM

rebuild:
	clean all
