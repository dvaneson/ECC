/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
*/

#include <stdio.h>
#include <stdbool.h>

struct e_curve {
    int p;
    int a;
    int b;
};

struct point {
    int x;
    int y;
};

bool PointEqual(struct point*, struct point *);
struct point* PointAdd(struct point*, struct point*, struct e_curve*);
struct point* DoubleAndAdd(struct point*, struct e_curve*, int);