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
