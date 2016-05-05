/* Rewrite 1_5 to use a function for fahr->celsius conversion */

#include <stdio.h>

#define UPPER  300
#define LOWER  0
#define STEP   20

float fahr_to_celsius(int fahr) {
	return (5.0/9.0)*(fahr-32);
}

int main()
{
	for (int fahr = LOWER; fahr <= UPPER; fahr += STEP)
		printf(" %3d %6.1f\n", fahr, fahr_to_celsius(fahr));
}

// $ clang ex_1_15.c && ./a.out
//    0  -17.8
//   20   -6.7
//   40    4.4
//   60   15.6
//   80   26.7
//  100   37.8
//  120   48.9
//  140   60.0
//  160   71.1
//  180   82.2
//  200   93.3
//  220  104.4
//  240  115.6
//  260  126.7
//  280  137.8
//  300  148.9
