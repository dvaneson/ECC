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

void point_copy(point *copy, point *src) {
    copy->x = src->x;
    copy->y = src->y;
    copy->at_inf = src->at_inf;
}

char* point_str(point *P) {
    char *str;
    if (P->at_inf)
        asprintf(&str, "Inf");
    else
        asprintf(&str, "(%d, %d)", P->x, P->y);

    return str;
}

void point_add(point *p1, point *p2, point *sum, ecurve *E) {
    int s, x, y;

    // Testing for "point at infinity" and point inverses
    if (p1->at_inf && p2->at_inf){
        sum->at_inf = true;
        return;
    } else if (p1->at_inf) {
        point_copy(sum, p2);
        return;
    } else if (p2->at_inf) {
        point_copy(sum, p1);
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
    x = mod((s * s) - p1->x - p2->x, E->p);
    y = mod(s * (p1->x - x) - p1->y, E->p);

    sum->x = x;
    sum->y = y;
    sum->at_inf = false;
}

void double_and_add(point *P, point *sum, ecurve *E, int num) {
    char buffer[65];
    itoa(num, buffer, 2);
    point_copy(sum, P);

    int i = 1;
    // printf("\t\tsum = %s\n", point_str(sum));
    while (buffer[i] != '\0') {
        point_add(sum, sum, sum, E);
        if (buffer[i] == '1')
            point_add(P, sum, sum, E);
        // printf("\t\tsum = %s\n", point_str(sum));
        ++i;
    }
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
