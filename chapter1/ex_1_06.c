#include <stdio.h>

int main()
{
	int c;

	while ((c = getchar()) != EOF) {
		printf("%d %d\n", c, c != EOF);
	}
	printf("%d %d\n", c, c != EOF);
}

// output:
// $ clang chapter1/ex_1_06.c && ./a.out
// a
// 97 1
// 10 1
// b
// 98 1
// 10 1
// abc
// 97 1
// 98 1
// 99 1
// 10 1
// -1 0
