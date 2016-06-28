/*

Write a recursive version of the function reverse(s), which reverses the 
string s in place.

*/

#include <stdio.h>
#include <string.h>

void reverse1(char s[], int start, int end);
void reverse1(char s[], int start, int end)
{
	if (start >= end)
		return;

	char tmp = s[end];
	s[end] = s[start];
	s[start] = tmp;

	reverse1(s, start + 1, end - 1);
}

void reverse(char s[]);
void reverse(char s[])
{
	reverse1(s, 0, (int) strlen(s) - 1);
}

void testreverse(char s[]);
void testreverse(char s[])
{
	printf("testreverse(\"%s\") = ", s);
	reverse(s);
	printf("\"%s\"\n", s);
}

int main()
{
	char a[] = "";
	testreverse(a);

	char b[] = "x";
	testreverse(b);

	char c[] = "xy";
	testreverse(c);

	char d[] = "xyz";
	testreverse(d);

	char e[] = "0123456789";
	testreverse(e);
}
