/*  Write the function any(s 1,s2), which returns the first location in the 
string s 1 where any character from the string s2 occurs, or -1 if s 1 contains 
no characters from s2. 

The stdlib function strpbrk does the same thing.*/

#include <stdio.h>

int any(char*, char*);
void test(char*, char*);

void test(char* a, char* b)
{
	printf("any(\"%s\", \"%s\") = %d\n", a, b, any(a, b));
}

int any(char* a, char* b)
{
	for (int i = 0; a[i] != '\0'; i++)
		for (int j = 0; b[j] != '\0'; j++)
			if (b[j] == a[i])
				return i;
	return -1;
}

int main()
{
	test("abc", "a");
	test("abc", "b");
	test("abc", "ba");
	test("abc", "c");
	test("abc", "cb");
	test("abc", "iupuwerwerb");
	test("abc", "d");
}

