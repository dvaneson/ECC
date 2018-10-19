/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "tools.h"

typedef struct {
    int p, a, b;
} ecurve;

typedef struct{
    int x, y;       // Coordinates for the point
    bool at_inf;    // Flag to signify if this is a point at infinity
} point;

int mod(int, int);
int mod_inverse(int, int);

bool point_equal(point *, point *);
bool point_inverse(point *, point *, int);
char* point_str(point *);

void point_add(point *, point *, point *, ecurve *);
void double_and_add(point *, point *, ecurve *, int);

char* itoa(int, char *, int);
