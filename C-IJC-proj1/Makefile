
CFLAGS=-O2 -std=c99 -Wall -pedantic

CC=gcc

all: primes steg-decode primes-i

primes-i:	eratosthenes.o primes.c error.o
		$(CC) $(CFLAGS) -DUSE_INLINE primes.c eratosthenes.o error.o -lm -o primes-i
primes: eratosthenes.o primes.c error.o
	$(CC) $(CFLAGS) primes.c eratosthenes.o error.o -lm -o primes
eratosthenes.o: eratosthenes.c eratosthenes.h error.o bit_array.h
	$(CC) $(CFLAGS) eratosthenes.c -lm -c -o eratosthenes.o
error.o: error.h error.c
	$(CC) $(CFLAGS) error.c -c -o error.o
ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) ppm.c -c -o ppm.o
steg-decode: steg-decode.c ppm.o error.o eratosthenes.o
	$(CC) $(CFLAGS) steg-decode.c ppm.o error.o eratosthenes.o -lm -o steg-decode
# smaze vytvorene soubory behem prekladu
clean:
		rm -f *.o primes

run: primes primes-i
	time ./primes;
	time ./primes-i;
