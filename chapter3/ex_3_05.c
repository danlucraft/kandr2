/*

Write the function itob(n,s,b) that converts the integer n into a
base b character representation in the string s.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void reverse(char s[]);
void reverse(char s[])
{
	int i, j, c;
	for (i = 0, j = (int) strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

char digit(unsigned int v);
char digit(unsigned int v)
{
	if (v < 10)
		return '0' + (char) v;
	else
		return 'A' + (char) (v - 10);
}

void itob(int n, char s[], int b);
void itob(int n, char s[], int b)
{
	// TODO: if b < 2 raise an error or whatever you do in C to raise an error here.

	int i = 0;
	int sign = n;

	do
	{
		s[i++] = digit(abs(n % b));
	}
	while(abs(n /= b) > 0);

	if (sign < 0)
		s[i++] = '-';

	s[i] = '\0';
	reverse(s);
}

void test_itob(int n, int b);
void test_itob(int n, int b)
{
	char s[] = "                            ";
	itob(n, s, b);
	printf("itob(%d, %u) = %s\n", n, b, s);
}

int main()
{
	printf("hex tests:\n");
	test_itob(13, 16);
	test_itob(20, 16);
	test_itob(200, 16);
	test_itob(255, 16);
	test_itob(-255, 16);
	test_itob(-65536, 16);

	printf("\nbinary tests:\n");
	test_itob(2, 2);
	test_itob(13, 2);
	test_itob(255, 2);
	test_itob(256, 2);
	test_itob(65536, 2);

	printf("\nternary tests:\n");
	test_itob(2, 3);
	test_itob(13, 3);
	test_itob(255, 3);
	test_itob(256, 3);
	test_itob(65536, 3);

	printf("\nedge case tests:\n");
	test_itob(0, 16);
	test_itob(1, 16);
	test_itob(-0, 16);
	test_itob(-1, 16);
	test_itob(INT_MIN, 16);
	test_itob(INT_MAX, 16);
	test_itob(INT_MIN, 10);
	test_itob(INT_MAX, 10);
}
