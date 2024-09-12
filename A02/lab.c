#include <stdio.h>
#include <math.h>

double power(int base, int exp)
{

    printf("%g", pow(base, exp));
    return 0;
}

double main()
{
    double result = power(2, 3);
    return result;
}
