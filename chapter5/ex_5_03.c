/*

Write a pointer version of the function strcat that we showed in Chapter 2:
strcat(s,t) copies the string t to the end of s.

*/

#include <stdio.h>

void my_strcat(char *s, char *t);
void my_strcat(char *s, char *t)
{
	while (*s) s++;
	while ((*s++ = *t++));
}

int main()
{
	char a[] = "Hello\0                        ";
	char b[] = ", world!";
	printf("strcat(\"%s\", \"%s\") = ", a, b);
	my_strcat(a, b);
	printf("\"%s\"", a);
}

/*

$ clang -Weverything chapter5/ex_5_03.c && ./a.out
strcat("Hello", ", world!") = "Hello, world!"

*/
