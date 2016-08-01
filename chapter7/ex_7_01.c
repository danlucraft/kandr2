/*

Write a program that converts upper case to lower or lower case to upper,
depending on the name it is invoked with, as found in argv[0].

--- 

Again, this is a silly exercise since it doesn't teach you ANYTHING about
what is being taught in this section.

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int __unused argc, char **argv)
{
	int (*case_changer)(int);
	int c;

	if (strcmp(*argv, "lowerize") == 0 || strcmp(*argv, "./lowerize") == 0)
		case_changer = tolower;
	else
		case_changer = toupper;

	while ((c = getchar()) != EOF)
		putchar(case_changer(c));

	return 0;
}

/*

$ cat chapter7/ex_7_01.test
Hello, World

$ clang -Weverything chapter7/ex_7_01.c && cat chapter7/ex_7_01.test | ./a.out
HELLO, WORLD

$ clang -Weverything chapter7/ex_7_01.c && cp a.out lowerize && cat chapter7/ex_7_01.test | ./lowerize
hello, world

*/
