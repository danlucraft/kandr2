// vim: noai:ts=4:sw=4

/*

Write a function rightrot(x, n) that returns the value of the integer x 
rotated to the right by n bit positions.

*/

#include <stdio.h>


unsigned int rightrot(unsigned int x, int n);
unsigned int rightrot(unsigned int x, int n)
{
    while(n-- > 0)
        x /= 2;
    return x;
}


void print_bits(unsigned int x);
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

void test_rightrot(unsigned int x, int n);
void test_rightrot(unsigned int x, int n)
{
    printf("rightrot(");
    print_bits(x);
    printf(", %d) = ", n);
    print_bits(rightrot(x, n));
    printf("\n");
}


int main()
{
    test_rightrot(245, 1);
    test_rightrot(245, 2);
    test_rightrot(245, 3);
    test_rightrot(245, 4);
    test_rightrot(245, 5);
    test_rightrot(245, 6);
}
