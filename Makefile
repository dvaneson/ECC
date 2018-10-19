CC = gcc

all: tools.a tools.o ecc.a ecc.o test_ecc.exe

tools.a: tools.o
	ar rcs $@ $^

tools.o: tools.c  tools.h
	$(CC) -c -o $@ $< -lm

ecc.a: ecc.o tools.o
	ar rcs $@ $^

ecc.o: ecc.c  ecc.h
	$(CC) -c -o $@ $< -lm

test_ecc.exe: test_ecc.o ecc.h ecc.a
	$(CC) $^ -o $@ -lm

test_ecc.o: test_ecc.c tools.h ecc.h
	$(CC) -c $< -o $@ -lm

clean:
	rm -f *.o *.a core

run: all
	./test_ecc.exe
