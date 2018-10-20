/*
    Donovan Ellison
    10-17-2018

    test_ecc.c - Code to test functions for ECC
*/

#include "ecc.h"

int main(int argc, char * argv[])
{
    bool test;
    point p1 = {1, 1}, p2 = {5, 1}, sum;
    ecurve E = {17, 2, 2};

    printf("\nElliptic Curve Crptography\n");

    /*==========================================================*/
    printf("Point Equal Tests\n");

    test = point_equal(&p1, &p2);
    printf("\tTest 1: Expected 0, Actual %d\n", test);

    p1 = (point){5, 1};
    test = point_equal(&p1, &p2);
    printf("\tTest 2: Expected 1, Actual %d\n", test);

    /*==========================================================*/
    printf("\nPoint Add Tests\n");

    // P + P
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // 6P + 6P
    p1 = (point){16, 13};
    p2 = (point){16, 13};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // P + 2P
    p1 = (point){5, 1};
    p2 = (point){6, 3};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // 2P + P
    p1 = (point){6, 3};
    p2 = (point){5, 1};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // 1P + 12P
    p1 = (point){5, 1};
    p2 = (point){0, 11};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // 12P + 1P
    p1 = (point){0, 11};
    p2 = (point){5, 1};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // Adding inverses, 5P + 14P
    p1 = (point){9, 16};
    p2 = (point){9, 1};
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // Adding point at infinity
    p1.at_inf = true;
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    // Adding two points at infinity
    p2.at_inf = true;
    point_add(&p1, &p2, &sum, &E);
    printf("\t%s + %s = %s\n", point_str(&p1), point_str(&p2), point_str(&sum));

    /*==========================================================*/
    printf("\nDouble And Add Tests\n");

    p1 = (point){5, 1, false};
    for (int i = 2; i <= 20; ++i) {
        double_and_add(&p1, &sum, &E, i);
        printf("\t%d%s = %s\n", i, point_str(&p1), point_str(&sum));
    }

    return 0;
}
