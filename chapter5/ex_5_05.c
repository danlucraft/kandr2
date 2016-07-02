/*

Write versions of the library functions strncpy, strncat, and strncmp, which 
operate on at most the first n characters of their argument strings. For
strncpy(s,t,n) copies at most n characters of t to s.

*/

#include <stdio.h>

#define puts printf("\n")

char *my_strncpy(char *s, const char *ct, int n);
char *my_strncpy(char *s, const char *ct, int n)
{
	while ((*s++ = *ct++) && n >= 0)
		n--;
	while (n >= 0) {
		*s++ = '\0';
		n--;
	}
	return s;
}

char *my_strncat(char *s, const char *ct, int n);
char *my_strncat(char *s, const char *ct, int n)
{
	while (*s) s++;
	while (*ct && n > 0) {
		*s++ = *ct++;
		n--;
	}
	*s = '\0';
	return s;
}

// return <0 if cs < ct, 0 if cs = ct, >0 if cs > ct
int my_strncmp(const char *cs, const char *ct, int n);
int my_strncmp(const char *cs, const char *ct, int n)
{
	// n > 1 because we don't want it to consume the last
	// character to compare
	for (; *cs == *ct && n > 1; cs++, ct++, n--)
		if (*cs == '\0')
			return 0;
	return *cs - *ct;
}

void test_strncpy(char *s, const char *ct, int n);
void test_strncpy(char *s, const char *ct, int n)
{
	printf("strncpy(\"%s\", \"%s\", %d) = ", s, ct, n);
	my_strncpy(s, ct, n);
	printf("\"%s\"\n", s);
}

void test_strncat(char *s, const char *ct, int n);
void test_strncat(char *s, const char *ct, int n)
{
	printf("strncat(\"%s\", \"%s\", %d) = ", s, ct, n);
	my_strncat(s, ct, n);
	printf("\"%s\"\n", s);
}

void test_strncmp(const char *s, const char *ct, int n);
void test_strncmp(const char *cs, const char *ct, int n)
{
	printf("strncmp(\"%s\", \"%s\", %d) = %d\n", cs, ct, n, 
			my_strncmp(cs, ct, n));
}

int main()
{
	char b[] = ", World!";

	char a[] = "Hello\0                       ";
	test_strncpy(a, b, 25);

	char a1[] = "Hello\0                    ";
	test_strncpy(a1, b, 3);
	puts;

	char a2[] = "Hello\0                    ";
	test_strncat(a2, b, 8);

	char a3[] = "Hello\0                    ";
	test_strncat(a3, b, 3);

	puts;

	char abc[] = "abc";
	char xyz[] = "xyz";
	char aac[] = "aac";
	char ab[] = "ab";
	char e[] = "";

	test_strncmp(abc, xyz, 10);
	test_strncmp(abc, abc, 10);
	puts;

	test_strncmp(abc, aac, 10);
	test_strncmp(abc, aac, 1);
	puts;

	test_strncmp(abc, ab, 10);
	test_strncmp(abc, ab, 2);
	puts;

	test_strncmp(e, ab, 2);
	test_strncmp(ab, e, 2);
	test_strncmp(e, e, 2);
}

/*

$ clang -Weverything chapter5/ex_5_05.c && ./a.out
strncpy("Hello", ", World!", 25) = ", World!"
strncpy("Hello", ", World!", 3) = ", Wor"

strncat("Hello", ", World!", 8) = "Hello, World!"
strncat("Hello", ", World!", 3) = "Hello, W"

strncmp("abc", "xyz", 10) = -23
strncmp("abc", "abc", 10) = 0

strncmp("abc", "aac", 10) = 1
strncmp("abc", "aac", 1) = 0

strncmp("abc", "ab", 10) = 99
strncmp("abc", "ab", 2) = 0

strncmp("", "ab", 2) = -97
strncmp("ab", "", 2) = 97
strncmp("", "", 2) = 0

*/
