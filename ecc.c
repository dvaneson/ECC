/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
*/

#define _GNU_SOURCE
#include "ecc.h"


int mod(int num, int mod) {
    int ret = num % mod;
    if (ret < 0)
        ret += mod;

    return ret;
}

int mod_inverse(int num, int mod) {
    int num_coeff, mod_coeff;
    egcd(num, mod, &num_coeff, &mod_coeff);

    return num_coeff;
}

bool point_equal(point *p1, point *p2) {
    if (p1->at_inf && p2->at_inf)
        return true;
    if (p1->at_inf || p2->at_inf)
        return false;

    return p1->x == p2->x && p1->y == p2->y;
}

bool point_inverse(point *p1, point *p2, int mod) {
    if (p1->at_inf || p2->at_inf)
        return false;

    return p1->x == p2->x && p1->y == (mod - p2->y);
}

void print_point_sum(point *p1, point *p2, point *sum) {
    char *p1_str, *p2_str, *sum_str;
    if (p1->at_inf)
        asprintf(&p1_str, "Inf");
    else
        asprintf(&p1_str, "(%d, %d)", p1->x, p1->y);

    if (p2->at_inf)
        asprintf(&p2_str, "Inf");
    else
        asprintf(&p2_str, "(%d, %d)", p2->x, p2->y);

    if (sum->at_inf)
        asprintf(&sum_str, "Inf");
    else
        asprintf(&sum_str, "(%d, %d)", sum->x, sum->y);

    printf("\t%s + %s = %s\n", p1_str, p2_str, sum_str);
}

void point_add(point *p1, point *p2, point *sum, ecurve *E) {
    int s;

    // Testing for "point at infinity" and point inverses
    if (p1->at_inf && p2->at_inf){
        sum->at_inf = true;
        return;
    } else if (p1->at_inf) {
        sum->x = p2->x;
        sum->y = p2->y;
        sum->at_inf = false;
        return;
    } else if (p2->at_inf) {
        sum->x = p1->x;
        sum->y = p1->y;
        sum->at_inf = false;
        return;
    } else if (point_inverse(p1, p2, E->p)) {
        sum->at_inf = true;
        return;
    }

    // Computing s depending on if the points are equal or not
    if (!point_equal(p1, p2))
        s = (p2->y - p1->y) * mod_inverse((p2->x - p1->x), E->p);
    else
        s = (3 * p1->x * p1->x + E->a) * mod_inverse((2 * p1->y), E->p);

    s = mod(s, E->p);
    sum->x = mod((s * s) - p1->x - p2->x, E->p);
    sum->y = mod(s * (p1->x - sum->x) - p1->y, E->p);
    sum->at_inf = false;
}

void double_and_add(point *P, point *T, ecurve *E, int d) {
    return;
}
