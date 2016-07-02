/*

Rewrite appropriate programs from earlier chapters and exercises with pointers 
instead of array indexing. Good possibilities include getline (Chapters 1 and 
4), atoi, itoa, (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex
(Chapter 4).

*/

#include <stdio.h>
#include <stdlib.h>

int get_line(char *s, int lim);
int get_line(char *s, int lim)
{
	int c = 0;
	int i;
	for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
		*s++ = (char) c;
	if (c == '\n') {
		*s++ = (char) c;
		++i;
	}
	*s = '\0';
	return i;
}

int my_atoi(char *s);
int my_atoi(char *s)
{
	int n = 0;
	for (; *s >= '0' && *s <= '9'; s++)
		n = 10 * n + (*s - '0');
	return n;
}

void reverse(char *s);
void reverse(char *s)
{
	char *t = s;
	while (*t++)
		; 
    // t is now one position past the \0
	t -= 2;
	while (t > s) {
		char tmp = *t;
		*t = *s;
		*s = tmp;
		s++;
		t--;
	}
}

void my_itoa(int n, char *s);
void my_itoa(int n, char *s)
{
	int sign = n;
	char *t = s;
	do {
		*s++ = (char) abs(n % 10) + '0';
	} while (abs(n /= 10) > 0);
	if (sign < 0)
		*s++ = '-';
	*s = '\0';
	printf("foo: %s\n", s);
	reverse(t);
}

// return pointer to t in s, or NULL if not there
char* strindex(char *s, char *t);
char* strindex(char *s, char *t)
{
	char *s1;
	char *t1;

	while (*s) {
		s1 = s;
		t1 = t;
		while (*t1 == *s1) {
			t1++;
			s1++;
		}
		if (*t1 == '\0')
			return (char *) s;
		s++;
	}
	return NULL;
}

void testreverse(char s[]);
void testreverse(char s[])
{
	printf("testreverse(\"%s\") = ", s);
	reverse(s);
	printf("\"%s\"\n", s);
}

void test_strindex(char *s, char *t);
void test_strindex(char *s, char *t)
{
	printf("strindex(\"%s\", \"%s\") = ", s, t);
	char *r = strindex(s, t);
	if (r == NULL)
		printf("NULL\n");
	else 
		printf("\"%s\"\n", r);
}

int main()
{
	char s[] = "                                   ";

	printf(">> ");
	get_line(s, 10);
	printf("get_line(s, 10) = \"%s\"\n", s);
	
	char i[] = "123";
	printf("my_atoi(\"%s\") = %d\n", i, my_atoi(i));

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

	char x[] = "                ";
	my_itoa(50904, x);
	printf("my_itoa(50904) = \"%s\"\n", x);

	test_strindex("foobarbaz", "foo");
	test_strindex("foobarbaz", "bar");
	test_strindex("foobarbaz", "baz");
	test_strindex("foobarbaz", "");
	test_strindex("foobarbaz", "food");
	test_strindex("foobarbaz", "doob");
}

/*

$ clang -Weverything chapter5/ex_5_06.c && ./a.out
>> 123
get_line(s, 10) = "123
"
my_atoi("123") = 123
testreverse("") = ""
testreverse("x") = "x"
testreverse("xy") = "yx"
testreverse("xyz") = "zyx"
testreverse("0123456789") = "9876543210"
foo:
my_itoa(50904) = "50904"
strindex("foobarbaz", "foo") = "foobarbaz"
strindex("foobarbaz", "bar") = "barbaz"
strindex("foobarbaz", "baz") = "baz"
strindex("foobarbaz", "") = "foobarbaz"
strindex("foobarbaz", "food") = NULL
strindex("foobarbaz", "doob") = NULL

*/
