/*

In a two's complement number representation, the first version of itoa
does not handle the largest negative number, i.e. -2**(wordsize-1).

*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

void reverse(char s[]);
void reverse(char s[])
{
	int c, i, j;

	for (i = 0, j = (int) strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = (char) c;
	}
}

void myitoa(int n, char s[]);
void myitoa(int n, char s[])
{
	int i, sign;

	sign = n;
	i = 0;
	do {
		s[i++] = (char) abs(n % 10) + '0';
	} while (abs(n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void itoa(int n, char s[]);
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

int main()
{
	int myInt;
	myInt = (int) 0xFFFFFFE2;
	printf("%d\n",myInt);

	char s[] = "                                          ";
	myitoa(18, s);
	printf("18: %s\n", s);

	int i2;
	i2 = (int) 0xFFFFFFFF;
	myitoa(i2, s);
	printf("%d: %s\n", i2, s);
	printf("-33 %% 10: %d\n", (-33) % 10);
	printf("abs(-33 %% 10 ): %d\n", abs((-33) % 10));
	printf("abs(-33 %% 20 ): %d\n", (-33 % 20));
	printf("abs(-33 %% 21 ): %d\n", (-33 % 21));

	printf("INT_MIN: %d, INT_MAX: %d\n", INT_MIN, INT_MAX);
	printf("INT_MIN - 1: %d, INT_MAX + 1: %d\n", INT_MIN - 1, INT_MAX + 1);


	printf("\nitoa from book:\n");
	itoa(INT_MIN + 1, s);
	printf("%d: %s\n", INT_MIN + 1, s);
	itoa(INT_MIN, s);
	printf("%d: %s\n", INT_MIN, s);

	printf("\nfixed itoa:\n");
	myitoa(INT_MIN + 1, s);
	printf("%d: %s\n", INT_MIN + 1, s);
	myitoa(INT_MIN, s);
	printf("%d: %s\n", INT_MIN, s);

}

/*

(in twos comp the smallest integer is 10000..  and 111111... is -1)
(i.e. INT_MIN is "bigger" than INT_MAX. This is because they both have n - 1
bits to represent the number except the positive representation includes 0
so has one less.)

This is because -INT_MIN would be INT_MAX + 1, so int can't represent
-INT_MIN. Instead it ends up being INT_MAX + 1 which wraps to INT_MIN anyway.
So the initial -n in itoa doesn't work (-INT_MIN can't be represented in int).

*/
