all: test

test: test.c
	gcc -o test test.c -I.

exam1: exam1.c
	gcc -o exam1 exam1.c -I. -lpthread

exam2: exam2.c
	gcc -o exam2 exam2.c -I. -lpthread

.PHONY: clean

clean:
	rm -f *.o
	rm -f test exam1
