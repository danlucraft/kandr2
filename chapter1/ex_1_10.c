#include <stdio.h>

int main()
{
	int c;

	while ((c = getchar()) != EOF) {
		if (c == '\t')
			printf("\\t");
		else if (c == '\b')
			printf("\\b");
		else if (c == '\\')
			printf("\\\\");
		else
			putchar(c);
	}
}
// $ clang -Weverything chapter1/ex_1_10.c && ./a.out
// a
// a
// 
// \t
// \
// \\
