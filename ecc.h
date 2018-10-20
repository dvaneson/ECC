/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
 */

#include <stdio.h>
#include <stdbool.h>

#include "tools.h"

typedef struct {
    int p, a, b;
} ecurve;

typedef struct{
    int x, y;       // Coordinates for the point
    bool at_inf;    // Flag to signify if this is a point at infinity
} point;

// Modulo operation that accounts for a negative remainder
int mod(int, int);
// Find the multiplicative inverse given a number and modulus
int mod_inverse(int, int);

// Test if two points are equal
bool point_equal(point *, point *);
// Test if two points are inverses in a given modulus
bool point_inverse(point *, point *, int);
// Return a string to represent a Point, either by (int, int) or Inf
char* point_str(point *);

// Elliptic curve point addition/doubling
void point_add(point *, point *, point *, ecurve *);
// Double and Add algorithm for eliptic curve point multiplication
void double_and_add(point *, point *, ecurve *, int);

char* itoa(int, char *, int);
