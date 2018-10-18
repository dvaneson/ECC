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
    printf("Test ");

    test = PointEqual(NULL, NULL);
    printf("%d\n", test);

    return 0;
}
