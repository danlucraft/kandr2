/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void itoa(int n, char s[]);
void itoa(int n, char s[])
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

void testitoa(int);
void testitoa(int n)
{
	char s[] = "                 ";
	itoa(n, s);
	printf("itoa(%d) = %s\n", n, s);
}


int main()
{
	testitoa(123);
	testitoa(56789);
	testitoa(1);
	testitoa(0);
}

