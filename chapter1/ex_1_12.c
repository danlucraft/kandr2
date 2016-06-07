/* Print each word in the input, one word per line */

#include <stdio.h>

int main()
{
	int c;
	int inword = 0;
	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			if (inword)
				putchar('\n');
			inword = 0;
		} else {
			putchar(c);
			inword = 1;
		}
	}
}

// $ clang ex_1_12.c && cat ex_1_12.c | ./a.out
// #include
// <stdio.h>
// int
// main()
// {
// int
// c;
// int
// inword
// =
// 0;
// while
// ((c
// =
// getchar())
// !=
// EOF)
// {
// if
// (c
// ==
// '
// '
// ||
// c
// ==
// '\t'
// ||
// c
// ==
// '\n')
// {
// if
// (inword)
// putchar('\n');
// inword
// =
// 0;
// }
// else
// {
// putchar(c);
// inword
// =
// 1;
// }
// }
// }
