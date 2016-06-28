#include <stdio.h>

#define dp(exp) printf(#exp " = %d\n", exp)
#define paste(front, back) front ## back

// this doesn't work as the argument e.g. front isn't expanded in frontback
//#define badpaste(front, back) frontback

int main()
{
	int x = 10;
	int y = 3;

	dp(x/y);

	int xy = 505;
	printf("xy = %d\n", paste(x, y));
}

/* 

$ clang -Weverything chapter4/section_4_11_2.c && ./a.out
x/y = 3

*/
