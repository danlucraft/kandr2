/* Write a program to copy its input to its output, replacing each string of
 * one or more blanks by a single blank. */

#include   <stdio.h>

int main()
{
	int  c;

	int prevspace = 0;

	// NB next line
	while ((c =        getchar()) != EOF) {
		if (c != ' ' || !prevspace) {
			putchar(c);
			prevspace = 0;
		}
		if (c == ' ')
			prevspace = 1;
	}
}

// $ clang chapter1/ex_1_09.c && head -n 15 chapter1/ex_1_09.c | ./a.out
///* Write a program to copy its input to its output, replacing each string of
// * one or more blanks by a single blank. */
//
//#include <stdio.h>
//
//int main()
//{
//	int c;
//
//	int prevspace = 0;
//
//	// NB next line
//	while ((c = getchar()) != EOF) {
//		if (c != ' ' || !prevspace) {
//			putchar(c);
