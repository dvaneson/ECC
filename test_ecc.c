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
    print_point_sum(&p1, &p2, &sum);

    // 2P + P
    p2 = (point){6, 3};
    point_add(&p1, &p2, &sum, &E);
    print_point_sum(&p1, &p2, &sum);

    // 3P + 3P
    p1 = (point){10, 6};
    p2 = (point){10, 6};
    point_add(&p1, &p2, &sum, &E);
    print_point_sum(&p1, &p2, &sum);

    // Adding inverses, 5P + 14P
    p1 = (point){9, 16};
    p2 = (point){9, 1};
    point_add(&p1, &p2, &sum, &E);
    print_point_sum(&p1, &p2, &sum);

    // Adding point at infinity
    p1.at_inf = true;
    point_add(&p1, &p2, &sum, &E);
    print_point_sum(&p1, &p2, &sum);

    // Adding two points at infinity
    p2.at_inf = true;
    point_add(&p1, &p2, &sum, &E);
    print_point_sum(&p1, &p2, &sum);

    /*==========================================================*/
    printf("\nDouble And Add Tests\n");

    return 0;
}
