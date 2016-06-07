/*

Write the function strrindex(s,t) which returns the position
of the rightmost occurrence of t in s, or -1 if there is none.

*/

#include <stdio.h>

int strrindex(char s[], char t[]);
int strrindex(char s[], char t[])
{
	int i, j;

	if (t[0] == '\0')
		return -1;

	int ix = -1;
	for (i = 0; s[i] != '\0'; i++) {
		for (j = 0; t[j] != '\0' && t[j] == s[i + j]; j++)
			;
		if (t[j] == '\0')
			ix = i;
	}
	return ix;
}

void test(char s[], char t[]);
void test(char s[], char t[])
{
	printf("strrindex(\"%s\", \"%s\") = %d\n", s, t, strrindex(s, t));
}

int main()
{
	test("abc", "b");
	test("abc abc", "abc");
	test("abc abc", "abc ");

	test("abc abc", "d");
	test("abc abc", "");
	test("", "d");
	test("", "");
}

/*

$ clang -g -Weverything chapter4/ex_4_01.c && ./a.out
strrindex("abc", "b") = 1
strrindex("abc abc", "abc") = 4
strrindex("abc abc", "abc ") = 0
strrindex("abc abc", "d") = -1
strrindex("abc abc", "") = -1
strrindex("", "d") = -1
strrindex("", "") = -1

*/
