
CFLAGS = -g -Wall -ansi -pedantic -O0

all: build

build: main.c ecc.h ecc.c
	gcc $(CFLAGS) -o prog.exe main.c ecc.h ecc.c

clean:
	rm -f *.o ecc

run: build
	./prog.exe
