/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
 */

#define _GNU_SOURCE
#include "ecc.h"


/*
 * Compute the modular of num, ensuring that num is non-negative
 */
int mod(int num, int modulo) {
    int ret = num % modulo;
    if (ret < 0)
        ret += modulo;

    return ret;
}

/*
 * Compute the modular multiplicate inverse of a number using
 * the extended Euclidean algorithm
 */
int mod_inverse(int num, int modulus) {
    int num_coeff, mod_coeff;
    // Ensure num is in [0, modulus)
    num = mod(num, modulus);
    egcd(num, modulus, &num_coeff, &mod_coeff);

    return num_coeff;
}

/*
 * Determine if two points are equal
 */
bool point_equal(point *p1, point *p2) {
    if (p1->at_inf && p2->at_inf)
        return true;
    if (p1->at_inf || p2->at_inf)
        return false;

    return p1->x == p2->x && p1->y == p2->y;
}

/*
 * Determine if two points are inverse to eachother given a modulus
 */
bool point_inverse(point *p1, point *p2, int modulus) {
    if (p1->at_inf || p2->at_inf)
        return false;

    return p1->x == p2->x && p1->y == (modulus - p2->y);
}

/*
 * Perform a deep copy between two points
 */
void point_copy(point *copy, point *src) {
    copy->x = src->x;
    copy->y = src->y;
    copy->at_inf = src->at_inf;
}

/*
 * Format a string to represent the point such that:
 *      (int, int) -> Any point on the curve
 *      Inf        -> The point at infinity
 *
 * Returns the new string
 */
char* point_str(point *P) {
    char *str;
    if (P->at_inf)
        asprintf(&str, "Inf");
    else
        asprintf(&str, "(%d, %d)", P->x, P->y);

    return str;
}

/*
 * Computes point addition/doubling for an elliptic curve
 *
 * p1, p2 -> The points to compute
 * sum    -> Where the point addition is stored
 * E      -> The elliptic curve
 */
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

/*
 * Function to compute the multiplication of a point using
 * the double and add algorithm
 *
 * P   -> The point being multiplied
 * sum -> Where the result is stored
 * E   -> The elliptic curve
 * num -> The amount of additions to perform
 */
void double_and_add(point *P, point *sum, ecurve *E, int num) {
    char buffer[65], *ptr;

    // Store the bit representation of num in buffer
    itoa(num, buffer, 2);
    point_copy(sum, P);

    // Start at the second point
    ptr = &buffer[1];
    while (*ptr != '\0') {
        point_add(sum, sum, sum, E);
        if (*ptr == '1')
            point_add(P, sum, sum, E);
        ptr++;
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
