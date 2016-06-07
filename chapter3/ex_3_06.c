/*

Write a version of itoa that pads the result with zeroes to the left

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char s[]);
void reverse(char s[])
{
	int i, j, c;
	for(i = 0, j = (int) strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = (char) c;
	}
}

void itoa(int n, char s[], int width);
void itoa(int n, char s[], int width)
{
	int sign = n;
	int i = 0;
	do {
		s[i++] = (char) abs(n % 10) + '0';
	} while (abs(n /= 10) > 0);

	int j;
	int extra = width - i;
	for (j = 0; j < extra; j++)
		s[i++] = '0';

	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';

	reverse(s);
}

void test_itoa(int n, int width);
void test_itoa(int n, int width)
{
	char s[] = "                         ";
	itoa(n, s, width);
	printf("itoa(%d, %d) = %s\n", n, width, s);
}

int main()
{
	test_itoa(13, 5);
	test_itoa(-13, 5);
	test_itoa(1, 3);
	test_itoa(134, 10);

	test_itoa(13, 0);
	test_itoa(13, -10);
}
