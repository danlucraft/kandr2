#include <stdio.h>

int main()
{
	int s, t, n;
	s = t = n = 0;

	int c;
	while ((c = getchar()) != EOF) {
		if (c == ' ')
			s++;
		else if (c == '\t')
			t++;
		else if (c == '\n')
			n++;
	}
	printf("spaces: %d, tabs: %d, newlines: %d\n", s, t, n);
}

//$ clang -Weverything chapter1/ex_1_08.c && ./a.out
//abc
//   f
//	x
//spaces: 3, tabs: 1, newlines: 3
