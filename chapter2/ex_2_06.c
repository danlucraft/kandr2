/* 

Write a function setbits (x,p,n,y) that returns x with the n bits that begin 
at position p set to the rightmost n bits of y, leaving the other bits 
unchanged. 

*/

#include <stdio.h>
#include <limits.h>

unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);

void print_bits(unsigned int x);

void test_getbits(unsigned x, int p, int n);
void test_setbits(unsigned x, int p, int n, unsigned y);

// getbits: from position p get n bits
// nb recall that this means get the n bits *beneath* p, not *above*
unsigned getbits(unsigned x, int p, int n)
{
	return (x >> (p+1-n)) & (unsigned) ~(~0 << n);
}

// returns x with the n bits that begin at position p set 
// to the rightmost n bits of y, leaving the other bits unchanged. 
unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	// create int with 1s only in first n columns
	unsigned int u = (unsigned) ~(~0 << n);
	
	// clear all bits in y except those we care about
	unsigned int y1 = y & u;
	
	// shift bits in y to correct position
	unsigned int y2 = y1 << (p - n + 1);
	
	// clear bits in x we want to get rid of
	unsigned int x1 = x & ~(u << (p - n + 1));
	
	// set the bits in x
	return x1 | y2;
}

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

void test_getbits(unsigned x, int p, int n)
{
	printf("getbits(");
	print_bits(x);
	printf(", %d, %d) = ", p, n);
	print_bits(getbits(x, p, n));
	printf("\n");
} 

void test_setbits(unsigned x, int p, int n, unsigned y)
{
	printf("setbits(");
	print_bits(x);
	printf(", %d, %d, ", p, n);
	print_bits(y);
	printf(") = \n        ");
	print_bits(setbits(x, p, n, y));
	printf("\n");
}

int main()
{
	test_getbits(20, 3, 4);
	test_getbits(20, 4, 4);
	test_getbits(20, 5, 4);
	
	printf("test setbits\n");
	test_setbits(121, 5, 4, 10000013);
	test_setbits(0, 3, 1, 1);
	test_setbits(0, 3, 2, 3);
	test_setbits(UINT_MAX, 3, 2, 0);
	test_setbits(UINT_MAX, 9, 5, 15);
}

/*

$ clang -g -Wno-newline-eof -Weverything ex_2_06.c && ./a.out
getbits(00000000000000000000000000010100, 3, 4) = 00000000000000000000000000000100
getbits(00000000000000000000000000010100, 4, 4) = 00000000000000000000000000001010
getbits(00000000000000000000000000010100, 5, 4) = 00000000000000000000000000000101
test setbits
setbits(00000000000000000000000001111001, 5, 4, 00000000100110001001011010001101) = 00000000000000000000000001110101
setbits(00000000000000000000000000000000, 3, 1, 00000000000000000000000000000001) = 00000000000000000000000000001000
setbits(00000000000000000000000000000000, 3, 2, 00000000000000000000000000000011) = 00000000000000000000000000001100
setbits(11111111111111111111111111111111, 3, 2, 00000000000000000000000000000000) = 11111111111111111111111111110011
setbits(11111111111111111111111111111111, 9, 5, 00000000000000000000000000001111) = 11111111111111111111110111111111

*/


