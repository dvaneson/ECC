/*
    Donovan Ellison
    10-17-2018

    ecc.h - Header for ECC functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"

int main(int argc, char * argv[])
{
    bool test;
    struct point P1 = {1, 1}, P2 = {1, 3};
    struct e_curve E = {17, 2, 2};

    printf("\nElliptic Curve Crptography\n");

    printf("Point Equal Tests\n");
    test = PointEqual(&P1, &P2);
    printf("\tTest 1: Expected 0, Actual %d\n", test);
    P1.y += 2;
    test = PointEqual(&P1, &P2);
    printf("\tTest 2: Expected 1, Actual %d\n", test);

    printf("\nPoint Add Tests\n");
    PointAdd(&P1, &P2, &E);
    P2.x = 3;
    PointAdd(&P1, &P2, &E);

    return 0;
}
