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
// $ clang 1_6.c && ./a.out
// a
// 97 1
// 10 1
// b
// 98 1
// 10 1
// c
// 99 1
// 10 1
// -1 0
