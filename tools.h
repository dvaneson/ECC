#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define	MAXDIGITS	100		/* maximum length bignum */ 

#define PLUS		1		/* positive sign bit */
#define MINUS		-1		/* negative sign bit */

typedef struct {
        char digits[MAXDIGITS];         /* represent the number */
		int signbit;			/* 1 if positive, -1 if negative */ 
        int lastdigit;			/* index of high-order digit */
} bignum;

#ifdef __cplusplus
    extern "C" {
#endif		

// skiena's bignum functions
void print_bignum(bignum *);
void int_to_bignum(int , bignum *);
void initialize_bignum(bignum *);
int max(int, int );
void add_bignum(bignum *, bignum *, bignum *);
void subtract_bignum(bignum *, bignum *, bignum *);
int compare_bignum(bignum *, bignum *);
void zero_justify(bignum *);
void digit_shift(bignum *, int );
void multiply_bignum(bignum *, bignum *, bignum *);
void divide_bignum(bignum *, bignum *, bignum *);
//end of skiena's bignum functions

//compute gcd(a,b) where a > b .ne 0 and integers
int gcd(int, int);
//extended Euclidean algorithm where a > b .ne 0 and integers
// returns gcd of a and b, and 2-element array containing s and table
// gcd(a,b) = x*a + y*b
int egcd(int, int, int*, int*);
//convert int to char string
void tostring(char *, int);
//gaussian elimination
int gaussElim(int, double *, double *, double *);
// list primes by trial division
void prtrdiv(int);
// variant on list primes by trial division  
void prtrdiv1();
//list prime by sieve of Eratosthenes
void esieve(int);
//list primes by multiplication algorithm
void primesbymult(int,int);
//utilities - swap, uniform, permute, and xmax
void swap(unsigned int *, unsigned int *);
unsigned int uniform(unsigned int,unsigned int); //returns a random integer 0 <= x <= m
//Knuth's random permutation by shuffle
void permute(unsigned int [], unsigned int);
int xmax(float *, int );
//unsigned int modular exponentiation
unsigned int modpower(unsigned int, unsigned int, unsigned int);
//long long modular multiplication and modular exponentiation
long long mulmod(long long, long long, long long);
long long modulo(long long, long long, long long);
//Miller test for pseudoprimes, Miller-Rabin probabilistic primality test
int miller(long long, long long);
int millerrabin(long long,int);
//Continued Fractions
void cfrac(int, int, int, int *);
void cfconvg(int, int *, double *);
int cfsqrt(int, int *);
void cf_factors(int, int, int *, int *, int *);
void cf_propfact(int, int, int, int *, int *, int *, int *);

#ifdef __cplusplus
    }
#endif	