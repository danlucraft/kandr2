/*

Write the function strend(s,t), which returns 1 if the string t
occurs at the end of the string s, and zero otherwise.

*/

#include <stdio.h>

int strend(char *s, char *t);
int strend(char *s, char *t)
{
	char *ps = s;
	char *pt = t;
	while (*ps) ps++;
	while (*pt) pt++;
	// now ps and pt point to \0

	// walk back while they match and we haven't passed the start of the strings
	while (*ps == *pt && ps > s && pt > t) {
		ps--;
		pt--;
	}

	// if we got all the way to the start of t, we matched all of it
	if (pt == t)
		return 1;
	else 
		return 0;
}

void test_strend(char *s, char *t);
void test_strend(char *s, char *t)
{
	printf("strend(\"%s\", \"%s\") = %d\n", s, t, strend(s, t));
}

int main()
{
	test_strend("foobar", "bar");
	test_strend("foobar", "baz");
	test_strend("foo", "oo");
	test_strend("f", "baz");
	test_strend("foo", "");
	test_strend("", "xxx");
	test_strend("", "");
}

/*

$ clang -Weverything chapter5/ex_5_04.c && ./a.out
strend("foobar", "bar") = 1
strend("foobar", "baz") = 0
strend("foo", "oo") = 1
strend("f", "baz") = 0
strend("foo", "") = 1
strend("", "xxx") = 0
strend("", "") = 1

*/
