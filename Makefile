
CFLAGS = -g -Wall -ansi -pedantic -O0

ecc: main.c ecc.h ecc.c
	gcc $(CFLAGS) -o ecc main.c ecc.h ecc.c

clean:
	rm -f *.o ecc

run: ecc
	./ecc
