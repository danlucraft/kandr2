// vim: noai:ts=4:sw=4
/* 

Write a function invert(x,p,n) that returns x with then bits that begin at 
position p inverted (i.e., 1 changed into 0 and vice versa), leaving the 
others unchanged.

*/

#include <stdio.h>
#include <limits.h>

void print_bits(unsigned int x);
unsigned invert(unsigned x, int p, int n);
void test_invert(unsigned x, int p, int n);

void print_bits(unsigned int x)
{
	char s[] = "00000000000000000000000000000000";
	for (int i = 0; i < 32; i++) {
		if ((x & 1) != 0)
			s[31 - i] = '1';
		else
			s[31 - i] = '0';
	    x = x >> 1;
	}
	printf("%s", s);
}

unsigned int invert(unsigned int x, int p, int n)
{
    // zero except for n rightmost bits 
    unsigned int u = ~((unsigned) ~0 << n);

    // move 1 bits into place
    unsigned int u1 = u << (p - n + 1);

    // XOR to flip bits
    return x ^ u1;
}

void test_invert(unsigned int x, int p, int n)
{
    printf("invert(");
    print_bits(x);
    printf(", %d, %d) = ", p, n);
    print_bits(invert(x, p, n));
    printf("\n");
}

int main()
{
    test_invert(0, 5, 3);
    test_invert(UINT_MAX - 16, 5, 3);
}
