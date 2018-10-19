/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
*/

#include <stdio.h>
#include <math.h>
#include "ecc.h"

bool PointEqual(struct point* P1, struct point* P2) {
    return P1->x == P2->x && P1->y == P2->y;
}

struct point* PointAdd(struct point* P1, struct point* P2, struct e_curve* E) {
    int s;
    if (!PointEqual(P1, P2))
        s = (P2->y - P1->y) / (P2->x - P1->x) % E->p;
    else
        s = (3 * P1->x * P1->x + E->a) / (2 * P1->y) % E->p;

    printf("s = %d", s);
    return NULL;
}

struct point* DoubleAndAdd(struct point* P, struct e_curve* E, int d) {
    return NULL;
}
